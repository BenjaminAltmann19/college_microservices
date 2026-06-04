//
// Created by Benji on 6/3/2026.
//

#define CPPHTTPLIB_USE_POLL
#define _WIN32_WINNT 0x0A00
#include "main.h"
#include "external/httplib.h"
#include "external/json.hpp"
#include <vector>
#include <iostream>
using namespace std;
using json = nlohmann::json;
using namespace httplib;



struct Enrollment {
    int studentId;
    int classId;
};

vector<Enrollment> enrollments;

int main() {
    httplib::Server server;

    server.Post("/enrollments", [](const httplib::Request& req, httplib::Response& res) {


    try {
        json body = json::parse(req.body);

        int studentId = body["studentId"];
        int classId = body["classId"];


        httplib::Client studentClient("localhost", 8001);

        auto studentResponse = studentClient.Get("/students/" + to_string(studentId));

        if (!studentResponse || studentResponse->status != 200) {
            res.status = 404;
            res.set_content(R"({"error":"Student not found"})",
                        "application/json");
            return;
        }

        httplib::Client classClient("localhost", 8002);

        auto classesResponse = classClient.Get("/classes/" + to_string(classId));

        if (!classesResponse || classesResponse->status != 200) {
            res.status = 404;
            res.set_content(R"({"error":"Class not found"})",
                        "application/json");
            return;
        }

        Enrollment newEnrollment;
        newEnrollment.studentId = studentId;
        newEnrollment.classId = classId;

        enrollments.push_back(newEnrollment);

        json response = {
                        {"success", true},
                        {"studentId", studentId},
                        {"classId", classId}
                    };

                    res.status = 201;
                    res.set_content(
                        response.dump(),
                        "application/json"
                    );
                }
                catch (...)
                {
                    res.status = 400;
                    res.set_content(
                        R"({"error":"Invalid request"})",
                        "application/json"
                    );
                }
            });

    server.Get("/enrollments",
        [](const httplib::Request&,
           httplib::Response& res)
        {
            json result = json::array();

            for (const auto& e : enrollments)
            {
                result.push_back({
                    {"studentId", e.studentId},
                    {"classId", e.classId}
                });
            }

            res.set_content(
                result.dump(),
                "application/json"
            );
        });

    std::cout
        << "Enrollment Service running on port 8003\n";

    server.listen("0.0.0.0", 8003);

    return 0;
}
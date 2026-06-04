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


struct Student {
    int id;
    string name;
    string email;
};

vector<Student> students = {
    {0, "Anna Jacobs", "anna@school.com"},
    {1, "Bob Sledge", "bob@school.com"}
};

int main() {
    Server server;

    server.Get("/students",
        [](const Request& req, Response& res) {
            string json = "[";

            for (size_t i = 0; i < students.size(); ++i) {
                json += "{";
                json += "\"id\":" + to_string(students[i].id) + ",";
                json += "\"name\":\"" + students[i].name + "\",";
                json += "\"email\":\"" + students[i].email + "\"";
                json += "}";

                if (i != students.size() - 1)
                    json += ",";
            }
                json += "]";

                res.set_content(json, "application/json");
        });



    server.Get(R"(/students/(\d+))",
        [](const Request& req, Response& res)
        {
            int studentId = std::stoi(req.matches[1]);

            for(const auto& student : students)
            {
                if(student.id == studentId)
                {
                    std::string json =
                        "{"
                        "\"id\":" + std::to_string(student.id) + ","
                        "\"name\":\"" + student.name + "\","
                        "\"email\":\"" + student.email + "\""
                        "}";

                    res.set_content(json, "application/json");
                    return;
                }
            }

            res.status = 404;
            res.set_content(
                "{\"error\":\"Student not found\"}",
                "application/json");
        });


    server.Post("/students", [](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);

            Student newStudent;
            newStudent.id = body["id"];
            newStudent.name = body["name"];
            newStudent.email = body["email"];

            students.push_back(newStudent);

            json response = {
               {"success", true},
               {"message", "Student created"},
               {"student", {
                   {"id", newStudent.id},
                   {"name", newStudent.name},
                   {"email", newStudent.email}
               }}
        };
                res.status = 201;
                res.set_content(response.dump(),"application/json");
        }
        catch (...) {
            res.status = 400;
            res.set_content( R"({"success":false,"error":"Invalid JSON"})",
                "application/json");
        }
    });

    std::cout << "Student Service running on port 8001\n";

    server.listen("0.0.0.0", 8001);

    return 0;
}
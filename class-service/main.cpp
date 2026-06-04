//
// Created by Benji on 6/3/2026.
//
#define CPPHTTPLIB_USE_POLL
#define _WIN32_WINNT 0x0A00
#include "main.h"
#include "external/httplib.h"
#include "external/json.hpp"
#include <vector>
using namespace std;
using json = nlohmann::json;
using namespace httplib;


struct Class {
    int id;
    string title;
    int capacity;
    int seatsRemaining;
};

vector<Class> classes = {
    {101, "Machine Learning", 25, 10},
    {102, "Game Programming", 30, 5}
};

int main() {
    Server server;

    server.Get("/classes",
        [](const Request& req, Response& res) {
            string json = "[";

            for (size_t i = 0; i < classes.size(); ++i) {
                json += "{";
                json += "\"id\":" + to_string(classes[i].id) + ",";
                json += "\"title\":\"" + classes[i].title + "\",";
                json += "\"email\":\"" + to_string(classes[i].capacity) + "\"";
                json += "\"email\":\"" + to_string(classes[i].seatsRemaining) + "\"";
                json += "}";

                if (i != classes.size() - 1)
                    json += ",";
            }
                json += "]";

                res.set_content(json, "application/json");
        });

    server.Get(R"(/classes/(\d+))",
        [](const Request& req, Response& res)
        {
            int classId = stoi(req.matches[1]);

            for(const auto& clas : classes)
            {
                if(clas.id == classId)
                {
                    std::string json =
                        "{"
                        "\"id\":" + to_string(clas.id) + ","
                        "\"name\":\"" + clas.title + "\","
                        "\"capacity\":\"" + to_string(clas.capacity) + "\""
                        "\"seatsRemaining\":\"" + to_string(clas.seatsRemaining) + "\""
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

    cout << "Class Service running on port 8002\n";

    server.listen("0.0.0.0", 8002);

    return 0;
}
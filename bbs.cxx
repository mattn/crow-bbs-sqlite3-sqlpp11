#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/sqlite3/connection.h>
#include <sqlpp11/sqlite3/connection_config.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <date/date.h>
#include "bbs.h"
#include "crow_all.h"

int
main(int argc, char* argv[]) {
  sqlpp::sqlite3::connection_config config;
  config.path_to_database = "bbs.db";
  config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
  //config.debug = true;
  sqlpp::sqlite3::connection db(config);
  bbsapp::Bbs bbs;

  crow::SimpleApp app;
  crow::mustache::set_base(".");

  CROW_ROUTE(app, "/")
  ([&]{
    crow::mustache::context ctx;
    int n = 0;
    for (const auto& row : db(
        select
          (bbs.id, bbs.text, bbs.created)
        .from(bbs).unconditionally()
        .order_by(bbs.created.desc())
        .limit(10u)
    )) {
      std::stringstream ss;
      ss << row.created;
      ctx["posts"][n]["id"] = row.id;
      ctx["posts"][n]["text"] = row.text;
      ctx["posts"][n]["created"] = ss.str();
      n++;
    }
    return crow::mustache::load("bbs.html").render(ctx);
  });

  CROW_ROUTE(app, "/post")
      .methods("POST"_method)
  ([&](const crow::request& req, crow::response& res){
    crow::query_string params(std::string("?") + req.body);
    const char* q = params.get("text");
    if (q == nullptr) {
      res = crow::response(400);
      res.write("bad request");
      res.end();
      return;
    }
    db(
      insert_into(bbs)
      .set(bbs.text = q));
    res = crow::response(302);
    res.set_header("Location", "/");
    res.end();
  });

  app.port(40081)
    //.multithreaded()
    .run();
  return 0;
}

// vim:set et sw=2 cino=>2,\:0:

#pragma once

#include <string>
#include <memory>
#include <functional>
#include <thread>
#include <atomic>

namespace httplib { class SSLServer; }

namespace WebAdmin
{
   class WebServer
   {
   public:
      WebServer();
      ~WebServer();

      bool Start(int preferredPort = 0);
      void Stop();
      int GetPort() const;
      bool IsRunning() const;

      using RouteHandler = std::function<void(const std::string& body, std::string& response, int& statusCode)>;
      void Get(const std::string& pattern, RouteHandler handler);
      void Post(const std::string& pattern, RouteHandler handler);
      void Put(const std::string& pattern, RouteHandler handler);
      void Delete(const std::string& pattern, RouteHandler handler);

   private:
      int FindAvailablePort(int preferred);
      bool GenerateSelfSignedCert();

      std::unique_ptr<httplib::SSLServer> server_;
      std::thread serverThread_;
      std::atomic<bool> running_{false};
      int port_{0};
      std::string certPath_;
      std::string keyPath_;
   };
}

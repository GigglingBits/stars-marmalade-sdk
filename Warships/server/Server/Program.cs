using System;
using System.Collections.Generic;
using System.Net;
using System.Threading;
using Server.Configuration;

namespace Server {
    public class Program {
        private static readonly Dictionary<int, TcpServer> Servers = new Dictionary<int, TcpServer>();

        private static void Main(string[] args) {
            Console.WriteLine("*********************************");
            Console.WriteLine("Server test application");
            Console.WriteLine("*********************************");

            var configuration = ServerConfigurationSection.GetCongiguration();
            StartServers(configuration.Listeners);

            Console.WriteLine("Press [Enter] to stop the server...");
            Console.Read();

            StopServers();
        }

        private static void StartServers(ListenerElementCollection listenerConfigs) {
            foreach (ListenerElement config in listenerConfigs) {
                int port = config.Port;
                IPAddress addr = config.LocalOnly ? IPAddress.Loopback : IPAddress.Any;
                string cert = config.CertificateName;

                TcpServer server;
                if (string.IsNullOrEmpty(cert)) {
                    server = new TcpServer(addr, port);
                }
                else {
                    server = new TcpServerSsl(addr, port, cert);
                }
                ThreadPool.QueueUserWorkItem(RunServer, server);
            }
        }

        private static void StopServers() {
            foreach (var server in Servers.Values) {
                server.StopListening();
            }
        }

        private static void RunServer(object stateInfo) {
            string threadName = Thread.CurrentThread.Name;
            try {
                var server = (TcpServer) stateInfo;
                Thread.CurrentThread.Name = string.Format("Listener on {0}:{1}", server.Address, server.Port);
                server.Listen();
            }
            finally {
                Thread.CurrentThread.Name = threadName;
            }
        }
    }
}
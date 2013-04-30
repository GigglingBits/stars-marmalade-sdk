using System;
using System.Net;
using System.Net.Security;
using System.Net.Sockets;
using System.Security.Authentication;
using System.Security.Cryptography.X509Certificates;
using System.Text;

namespace Server {
    public class TcpServerSsl : TcpServer {
        private string _certName;

        public TcpServerSsl(IPAddress address, int port, string certName) : base(address, port) {
            _certName = certName;
        }

        protected override void ProcessClient(object client) {
            var newClient = (TcpClient) client;

            // Buffer for reading data
            var bytes = new byte[1024];
            string clientData = null;


            var sslStream = new SslStream(newClient.GetStream());
            sslStream.AuthenticateAsServer(GetServerCert(_certName), false, SslProtocols.Default, true);


            // get the stream to talk to the client over
            //NetworkStream ns = newClient.GetStream();


            // Loop to receive all the data sent by the client.
            int bytesRead = 0;
            while ((bytesRead = sslStream.Read(bytes, 0, bytes.Length)) != 0) {
                // Translate data bytes to an ASCII string.
                clientData = Encoding.ASCII.GetString(bytes, 0, bytesRead);
                Console.WriteLine("Client says: {0}", clientData);


                // Thank them for their input
                bytes = Encoding.ASCII.GetBytes("Thanks call again!");

                // Send back a response.
                sslStream.Write(bytes, 0, bytes.Length);
            }

            // stop talking to client
            sslStream.Close();
            newClient.Close();
        }

        private static X509Certificate GetServerCert(string subjectName) {
            var store = new X509Store(StoreName.My, StoreLocation.LocalMachine);
            store.Open(OpenFlags.ReadOnly);
            X509CertificateCollection certificate = store.Certificates.Find(X509FindType.FindBySubjectName, subjectName,
                                                                            true);

            if (certificate.Count > 0) {
                return (certificate[0]);
            }
            else {
                return (null);
            }
        }
    }
}
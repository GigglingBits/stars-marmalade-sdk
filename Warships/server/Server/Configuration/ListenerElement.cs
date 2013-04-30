using System;
using System.Configuration;

namespace Server.Configuration {
    /// <summary>
    /// Defined the listener
    /// </summary>
    public class ListenerElement : ConfigurationElement {
        /// <summary>
        /// TCP port to listen on
        /// </summary>
        [ConfigurationProperty("port", DefaultValue = "0", IsRequired = true, IsKey = true)]
        [IntegerValidator(ExcludeRange = false, MinValue = 0, MaxValue = 65535)]
        public int Port {
            get { return (int) this["port"]; }
            set { this["port"] = value; }
        }

        /// <summary>
        /// Support only local connections. Useful for testing,
        /// </summary>
        [ConfigurationProperty("localOnly", DefaultValue = "false", IsRequired = false)]
        public Boolean LocalOnly {
            get { return (Boolean) this["localOnly"]; }
            set { this["localOnly"] = value; }
        }

        /// <summary>
        /// Name of the server certificate; implies that the listener is SSL encrypted
        /// </summary>
        [ConfigurationProperty("serverCertificate", DefaultValue = "", IsRequired = false)]
        [StringValidator(InvalidCharacters = "~!@#$%^&*()[]{}/;'\"|\\", MinLength = 0, MaxLength = 60)]
        public String CertificateName {
            get { return (String) this["serverCertificate"]; }
            set { this["serverCertificate"] = value; }
        }
    }
}
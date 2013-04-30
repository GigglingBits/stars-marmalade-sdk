using System;
using System.Configuration;

namespace Server.Configuration {
    public class ServerConfigurationSection : ConfigurationSection {
        [ConfigurationProperty("listeners", Options = ConfigurationPropertyOptions.IsRequired)]
        public ListenerElementCollection Listeners {
            get { return (ListenerElementCollection) this["listeners"]; }
        }

        public static ServerConfigurationSection GetCongiguration() {
            return ConfigurationManager.GetSection("serverConfiguration") as ServerConfigurationSection;
        }
    }
}
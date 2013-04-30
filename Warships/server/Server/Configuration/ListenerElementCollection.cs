using System;
using System.Configuration;

namespace Server.Configuration {
    [ConfigurationCollection(typeof (ListenerElement), AddItemName = "listener",
        CollectionType = ConfigurationElementCollectionType.BasicMap)]
    public class ListenerElementCollection : ConfigurationElementCollection {
        protected
            override ConfigurationElement CreateNewElement() {
            return new ListenerElement();
        }

        protected override object GetElementKey(ConfigurationElement element) {
            if (element == null) {
                throw new ArgumentNullException("element");
            }

            return ((ListenerElement) element).Port;
        }
    }
}
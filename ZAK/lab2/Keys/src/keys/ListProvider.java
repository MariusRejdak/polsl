
package keys;

import java.security.Provider;
import java.security.Security;
import java.util.Iterator;

/**
 *
 * @author ZAK
 */
public class ListProvider {
    public static void main(String[] args)
    {
            Provider provider = Security.getProvider("SUN");

            Iterator it = provider.keySet().iterator();

            while (it.hasNext()) {
                String entry = (String)it.next();

                if(entry.startsWith("Alg.Alias."))
                {
                    entry = entry.substring("Alg.Alias.".length());
                }

                String factoryClass = entry.substring(0, entry.indexOf('.'));
                String name = entry.substring(factoryClass.length()+1);

                System.out.println(factoryClass + ": " + name);
            }
        }
}

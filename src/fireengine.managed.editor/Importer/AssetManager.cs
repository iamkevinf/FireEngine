using System;
using System.Collections.Generic;
using System.Text;

namespace FireEditor
{

    public static class AssetManager
    {
        static Dictionary<string, object> pool = new Dictionary<string, object>();

        public static void AddOrUpAsset(string fullname, object asset)
        {
            if (pool.ContainsKey(fullname))
                pool[fullname] = asset;
            else
                pool.Add(fullname, asset);
        }
    }
}

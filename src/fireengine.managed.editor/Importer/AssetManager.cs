using System;
using System.Collections.Generic;
using System.Text;

namespace FireEditor
{
    public enum AssetType
    {
        Texture,
        Shader,
        Material,
        Mesh
    }

    public interface iAsset
    {
        AssetType type { get; }

        void Import();
    }

    public static class AssetManager
    {
        static Dictionary<AssetType, iAsset> pool = new Dictionary<AssetType, iAsset>();

        public static void RegisterAsset(iAsset asset)
        {
            if (pool.ContainsKey(asset.type))
                return;

            pool.Add(asset.type, asset);
        }
    }
}

using ImGuiNET;
using McMaster.NETCore.Plugins;
using System;

namespace FireEngine
{
    class Program
    {
        static void Main(string[] args)
        {
            App app = new App();
            app.Run();
        }

        //static void OnInit()
        //{
        //    Console.WriteLine("OnInit");
        //    string pluginPathAbs = System.IO.Path.Combine(AppContext.BaseDirectory, "TestPlugin.dll");
        //    LoadPlugin(pluginPathAbs);
        //}

        //static void OnGui()
        //{
        //    ImGui.ShowDemoWindow();

        //    ImGui.ProgressBar(0.5f);

        //    if(s_reloading)
        //    {
        //        if(s_reloadLoader != null)
        //        {
        //            InvokePlugin(s_reloadLoader);
        //            s_plugin_loader = s_reloadLoader;
        //            s_reloading = false;
        //            s_reloadLoader = null;
        //        }
        //    }


        //    if (s_plugin_loader != null)
        //    {
        //        try
        //        {
        //            InvokePlugin(s_plugin_loader);
        //        }
        //        catch(Exception e)
        //        {
        //            Console.WriteLine(e.ToString());
        //        }
        //    }

        //}

        //static void OnExit()
        //{
        //    Console.WriteLine("OnExit");
        //}

        //static PluginLoader s_plugin_loader = null;
        //static void LoadPlugin(string pluginPath)
        //{
        //    var loader = PluginLoader.CreateFromAssemblyFile(pluginPath,
        //        config => config.EnableHotReload = true);

        //    loader.Reloaded += ShowPluginInfo;

        //    s_plugin_loader = loader;
        //}

        //static bool s_reloading = false;
        //static PluginLoader s_reloadLoader = null;
        //private static void ShowPluginInfo(object sender, PluginReloadedEventArgs eventArgs)
        //{
        //    s_reloading = true;
        //    s_reloadLoader = eventArgs.Loader;
        //    s_plugin_loader = null;
        //}

        //private static void InvokePlugin(PluginLoader loader)
        //{
        //    var assembly = loader.LoadDefaultAssembly();
        //    assembly
        //        .GetType("FakeUnityPlugin.Program", throwOnError: true)
        //        !.GetMethod("Entry")
        //        !.Invoke(null, null);
        //}

    }
}

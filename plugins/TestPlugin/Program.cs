using ImGuiNET;

namespace FakeUnityPlugin
{
    public class Program
    {
        public static void Entry()
        {
            ImGui.ProgressBar(0.2f);
            ImGui.ProgressBar(0.3f);
            ImGui.ProgressBar(0.4f);
            ImGui.ProgressBar(0.5f);
        }

    }
}

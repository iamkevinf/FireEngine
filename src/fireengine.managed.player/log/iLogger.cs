namespace FireEngine
{
    public interface iLogger
    {
        void Log(string str);
        void LogFormat(string fmt, params object[] paramList);
        void LogWarning(string str);
        void LogWarningFormat(string fmt, params object[] paramList);
        void LogError(string str);
        void LogErrorFormat(string fmt, params object[] paramList);
    }
}

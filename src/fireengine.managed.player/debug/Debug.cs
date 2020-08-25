namespace FireEngine
{
    public class DefaultLogger : iLogger
    {
        public void Log(string str)
        {
            System.Console.WriteLine(str);
        }

        public void LogError(string str)
        {
            System.Console.WriteLine(str);
        }

        public void LogErrorFormat(string fmt, params object[] paramList)
        {
            System.Console.WriteLine(string.Format(fmt, paramList));
        }

        public void LogFormat(string fmt, params object[] paramList)
        {
            System.Console.WriteLine(string.Format(fmt, paramList));
        }

        public void LogWarning(string str)
        {
            System.Console.WriteLine(str);
        }

        public void LogWarningFormat(string fmt, params object[] paramList)
        {
            System.Console.WriteLine(string.Format(fmt, paramList));
        }
    }

    public static class Debug
    {
        static iLogger logger = new DefaultLogger();

        public static void SetLogger(iLogger _logger)
        {
            logger = _logger;
        }

        public static void Log(string str)
        {
            logger.Log(str);
        }
        public static void LogFormat(string fmt, params object[] paramList)
        {
            logger.LogFormat(fmt, paramList);
        }

        public static void LogWarning(string str)
        {
            logger.LogWarning(str);
        }
        public static void LogWarningFormat(string fmt, params object[] paramList)
        {
            logger.LogWarningFormat(fmt, paramList);
        }
        public static void LogError(string str)
        {
            logger.LogError(str);
        }
        public static void LogErrorFormat(string fmt, params object[] paramList)
        {
            logger.LogErrorFormat(fmt, paramList);
        }
    }
}

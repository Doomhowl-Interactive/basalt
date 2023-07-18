using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace launcher
{
    internal static class Program
    {
        private static void OpenCrashWindow(Exception e)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new CrasherForm(e));
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            try
            {
                ProcessStarter.StartGameProcess();
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                OpenCrashWindow(e);
            }

        }
    }
}

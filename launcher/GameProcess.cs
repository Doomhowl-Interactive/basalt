using System;
using System.Diagnostics;

namespace launcher
{
    internal class GameProcess : Process
    {
        private string log;

        private void ProcessMessage(object sender, DataReceivedEventArgs e)
        {
            log += e.Data + "\n";
            Console.WriteLine(e.Data);
        }

        public GameProcess(string path)
        {
            StartInfo.FileName = path;
            // TODO: add support for basalt command line arguments

            StartInfo.UseShellExecute = false;
            StartInfo.RedirectStandardOutput = true;
            StartInfo.RedirectStandardError = true;

            Start();

            // link stdout and stderr
            ErrorDataReceived += ProcessMessage;
            OutputDataReceived += ProcessMessage;

            BeginOutputReadLine();
            BeginErrorReadLine();

            WaitForExit();

            if (ExitCode != 0)
            {
                throw new Exception("Game exited with code " + ExitCode + "\n" + log);
            }
        }

        public static GameProcess StartGameProcess()
        {
            var exe = FileUtils.FindGameExecutable();
            return new GameProcess(exe);
        }
    }
}

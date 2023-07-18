using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace launcher
{
    internal class ProcessStarter
    {

        public static void StartGameProcess()
        {
            var exe = FileUtils.FindGameExecutable();
            throw new Exception("Executable is " + exe);
        }
    }
}

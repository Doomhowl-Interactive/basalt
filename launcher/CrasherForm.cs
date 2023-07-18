using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace launcher
{
    public partial class CrasherForm : Form
    {
        public CrasherForm(Exception error)
        {
            InitializeComponent();
            string output = error.ToString();
            Console.Write(output);
            errorBox.Text = output;
        }
    }
}

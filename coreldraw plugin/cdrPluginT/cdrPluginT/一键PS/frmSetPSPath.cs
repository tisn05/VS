using cdrPluginT.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace cdrPluginT
{
    public partial class frmSetPSPath : Form
    {
        public frmSetPSPath()
        {
            InitializeComponent();
        }

        private void frmSetPSPath_Load(object sender, EventArgs e)
        {
            textBox1.Text = Settings.Default.PSPath;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                textBox1.Text = openFileDialog1.FileName;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (!File.Exists(textBox1.Text))
            {
                
            }
            else
            {
                Settings.Default.PSPath = textBox1.Text;
                Settings.Default.Save();
            }
            Close();
        }
    }
}

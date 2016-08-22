using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace cdrPluginT
{
    public partial class frmTJAsk : Form
    {
        public frmTJAsk()
        {
            InitializeComponent();
        }

        public bool Remember = false;
        public bool yesno = true;
        public String FileName = "";
        private void frmTJAsk_Load(object sender, EventArgs e)
        {
            label1.Text = label1.Text.Replace("<文件名>", FileName);
            if (Remember)
            {
                if (yesno)
                    DialogResult = System.Windows.Forms.DialogResult.OK;
                else
                    DialogResult = System.Windows.Forms.DialogResult.Cancel;
                Close();
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            Remember = checkBox1.Checked;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            yesno = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            yesno = false;
        }
    }
}

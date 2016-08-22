using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace cdrTest
{
    public partial class frmInput : Form
    {
        public frmInput()
        {
            InitializeComponent();
        }
        public String value = "";
        private void frmInput_Load(object sender, EventArgs e)
        {
            this.textBox1.Text = value;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.value = textBox1.Text;
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }
    }
}

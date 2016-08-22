using Corel.Interop.VGCore;
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
    public partial class frmShowHide : BaseForm
    {
        public frmShowHide()
        {
            InitializeComponent();
        }

        public frmShowHide(Corel.Interop.VGCore.Application app)
            : base(app)
        {
            InitializeComponent();
        }

        private void frmShowHide_Load(object sender, EventArgs e)
        {
            application.OnApplicationEvent += application_OnApplicationEvent;
        }

        void application_OnApplicationEvent(string EventName, ref object[] Parameters)
        {
            Console.WriteLine(EventName);
            if (EventName.Equals("ActiveViewChange") || EventName.Equals("PageCountChange") || EventName.Equals("ActivePageChange") || EventName.Equals("SelectionChange") || EventName.Equals("StateChange"))
            {
                radioButton2.Enabled = application.ActiveDocument.SelectionRange.Count > 0;
                if (!radioButton2.Enabled && radioButton2.Checked)
                {
                    radioButton1.Checked = true;
                }
            }
        }

        private void button_Hide_Click(object sender, EventArgs e)
        {
            if (radioButton2.Checked)
            {
                application.ActiveDocument.SelectionRange.Hide();
            }
            else if (radioButton1.Checked)
            {
                application.ActivePage.Shapes.All().Hide();
            }
            else
            {
                foreach (Page p in application.ActiveDocument.Pages)
                {
                    p.Shapes.All().Hide();
                }
            }
        }

        private void frmShowHide_FormClosing(object sender, FormClosingEventArgs e)
        {
            application.OnApplicationEvent -= application_OnApplicationEvent;
        }

        private void button_Show_Click(object sender, EventArgs e)
        {
            if (radioButton2.Checked)
            {
                application.ActiveDocument.SelectionRange.Show();
            }
            else if (radioButton1.Checked)
            {
                application.ActivePage.Shapes.All().Show();
            }
            else
            {
                foreach (Page p in application.ActiveDocument.Pages)
                {
                    p.Shapes.All().Show();
                }
            }
        }

    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Corel.Interop.VGCore;

namespace cdrPluginT
{
    public partial class frmTest : BaseForm
    {
        public frmTest()
        {
            InitializeComponent();
        }

        public frmTest(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.application = app;
            app.OnApplicationEvent += app_OnApplicationEvent;
        }

        void app_OnApplicationEvent(string EventName, ref object[] Parameters)
        {
            listBox1.Items.Add(EventName);
            listBox1.SelectedIndex = listBox1.Items.Count - 1;
        }

        private void frmTest_FormClosed(object sender, FormClosedEventArgs e)
        {
            application.OnApplicationEvent -= app_OnApplicationEvent;
        }
    }
}

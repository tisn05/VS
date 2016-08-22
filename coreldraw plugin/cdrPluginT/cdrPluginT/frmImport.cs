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
    public partial class frmImport : BaseForm
    {
        public frmImport()
        {
            InitializeComponent();
        }

        public frmImport(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.application = app;
        }

        private void frmImport_Load(object sender, EventArgs e)
        {
            cmdName = "Import";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                StartEvent();
                foreach (String f in openFileDialog1.FileNames)
                {
                    application.ActiveDocument.AddPages(1);
                    application.ActiveDocument.Pages.Last.Activate();
                    application.ActiveDocument.ActiveLayer.Import(f);
                }
                EndEvent();
            }
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }
    }
}

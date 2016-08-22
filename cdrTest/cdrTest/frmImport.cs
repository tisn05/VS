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
    public partial class frmImport : BaseForm
    {
        public frmImport()
        {
            InitializeComponent();
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
                    app.ActiveDocument.AddPages(1);
                    app.ActiveDocument.Pages.Last.Activate();
                    app.ActiveDocument.ActiveLayer.Import(f);
                }
                EndEvent();
            }
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }
    }
}

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
    public partial class frmRemoveViews : BaseForm
    {
        public frmRemoveViews()
        {
            InitializeComponent();
        }

        public frmRemoveViews(Corel.Interop.VGCore.Application app)
            : base(app)
        {
            InitializeComponent();
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Corel.Interop.VGCore;

namespace cdrTest
{
    public partial class frmGuideLine : BaseForm
    {
        public frmGuideLine()
        {
            InitializeComponent();
            this.cmdName = "颜色";
        }

        private void frmColor_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            Shapes sr = app.ActiveDocument.SelectableShapes;
            foreach (Shape s in sr)
            {
                if (s.Type == cdrShapeType.cdrGuidelineShape)
                {
                    s.Delete();
                }
            }
        }
    }
}

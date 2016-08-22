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
    public partial class frmGuideLine : BaseForm
    {
        public frmGuideLine()
        {
            InitializeComponent();
            this.cmdName = "颜色";
        }
        public frmGuideLine(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.cmdName = "删除辅助线";
            this.application = app;
        }

        private void frmColor_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            Shapes sr = application.ActiveDocument.SelectableShapes;
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

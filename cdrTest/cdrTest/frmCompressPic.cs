using Corel.Interop.VGCore;
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
    public partial class frmCompressPic : BaseForm
    {
        public frmCompressPic()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            StartEvent();
            foreach(Page p in app.ActiveDocument.Pages)
            {
                p.Activate();
                ShapeRange sr = p.FindShapes(Type: cdrShapeType.cdrBitmapShape);
                foreach(Shape s in sr)
                {
                    s.ConvertToBitmapEx(cdrImageType.cdrCMYKColorImage, false, true, 300, cdrAntiAliasingType.cdrNormalAntiAliasing, true, false, 95);
                }
            }
            EndEvent();
        }
    }
}

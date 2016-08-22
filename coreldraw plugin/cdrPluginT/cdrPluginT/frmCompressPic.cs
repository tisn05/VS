using Corel.Interop.VGCore;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Bitmap = Corel.Interop.VGCore.Bitmap;

namespace cdrPluginT
{
    public partial class frmCompressPic : BaseForm
    {
        public frmCompressPic()
        {
            InitializeComponent();
        }

        public frmCompressPic(Corel.Interop.VGCore.Application app)
            : base(app)
        {
            InitializeComponent();
            this.cmdName = "压缩图片";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            cmdName = "压缩图片";
            int px = -1, cpx = -1;
            int.TryParse(comboBox1.Text, out px);
            int.TryParse(comboBox2.Text, out cpx);
            if (px == -1 || cpx == -1)
            {
                MessageBox.Show("请输入正确的数字！！");
                return;
            }
            StartEvent(true, true);
            foreach (Page p in application.ActiveDocument.Pages)
            {
                p.Activate();
                Compress(p.Shapes, px, cpx);
            }
            EndEvent();
            MessageBox.Show("图片压缩完成！");
        }

        private void Compress(Shapes ss, int px, int cpx)
        {
            foreach (Shape s in ss.FindShapes(null, cdrShapeType.cdrBitmapShape))
            {
                Bitmap b = s.Bitmap;
                if (b.ResolutionX <= px && b.ResolutionY <= px)
                    continue;
                s.ConvertToBitmapEx(b.Mode, false, true, cpx, cdrAntiAliasingType.cdrNormalAntiAliasing, true, false, 95);
            }
            foreach (Shape s in ss.FindShapes(Query: "@com.PowerClip <> null"))
            {
                s.PowerClip.EnterEditMode();
                Compress(application.ActiveLayer.Shapes, px, cpx);
                s.PowerClip.LeaveEditMode();
            }
        }

        private void ColorMode(Shapes ss, bool bl)
        {
            foreach (Shape s in ss.FindShapes(null, cdrShapeType.cdrBitmapShape))
            {
                Bitmap b = s.Bitmap;
                cdrImageType it, tit;
                if (bl)
                {
                    it = cdrImageType.cdrCMYKColorImage;
                    tit = cdrImageType.cdrRGBColorImage;
                }
                else
                {
                    it = cdrImageType.cdrRGBColorImage;
                    tit = cdrImageType.cdrCMYKColorImage;
                }
                if (b.Mode == it)
                {
                    s.ConvertToBitmapEx(tit, false, b.Transparent, b.ResolutionX);
                }
            }
            foreach (Shape s in ss.FindShapes(Query: "@com.PowerClip <> null"))
            {
                s.PowerClip.EnterEditMode();
                ColorMode(application.ActiveLayer.Shapes, bl);
                s.PowerClip.LeaveEditMode();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            cmdName = "RGB转CMYK";
            StartEvent(true, true); 
            foreach (Page p in application.ActiveDocument.Pages)
            {
                p.Activate();
                ColorMode(p.Shapes, false);
            }
            EndEvent();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            cmdName = "CMKY转RGB";
            StartEvent(true, true);
            foreach (Page p in application.ActiveDocument.Pages)
            {
                p.Activate();
                ColorMode(p.Shapes, true);
            }
            EndEvent();
        }
    }
}

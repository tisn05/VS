using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Corel.Interop.VGCore;

namespace cdrPluginT
{
    public partial class frmCropMark : BaseForm
    {
        public frmCropMark()
        {
            InitializeComponent();
            this.cmdName = "制作裁切线";
        }

        public frmCropMark(Corel.Interop.VGCore.Application app):base(app)
        {
            InitializeComponent();
            this.cmdName = "制作裁切线";
        }
        private Shape DrawLine(double x1, double y1, double x2, double y2, bool white = false)
        {
            Shape s;
            s = application.ActiveLayer.CreateLineSegment(x1, y1, x2, y2);
            s.Outline.Width = 0.075;
            if (white)
            {
                s.Outline.Color.CMYKAssign(0, 0, 0, 0);
            }
            else
            {
                s.Outline.Color.RegistrationAssign();
            }
            return s;
        }
        private void DoDrawCropMarks()
        {
            double X, y, cx, cy;
            double sx, sy, dx, ty;
            double Off, l, Cu;
            long N;
            ShapeRange sr = new ShapeRange();
            Shape s, s1;
            ShapeRange AllShapes = new ShapeRange();
            this.StartEvent();
            application.ActiveDocument.Unit = cdrUnit.cdrMillimeter;
            application.ActiveDocument.PreserveSelection = false;
            application.ActiveDocument.Selection().GetBoundingBox(out X, out y, out cx, out cy, false);
            Off = (double)nudOffset.Value;
            l = (double)nudLen.Value;
            sr.Add(DrawLine(X - Off - l, y, X - Off, y));
            sr.Add(DrawLine(X - Off - l, y + cy, X - Off, y + cy));
            sr.Add(DrawLine(X + cx + Off, y, X + cx + Off + l, y));
            sr.Add(DrawLine(X + cx + Off, y + cy, X + cx + Off + l, y + cy));
            sr.Add(DrawLine(X, y - Off - l, X, y - Off));
            sr.Add(DrawLine(X + cx, y - Off - l, X + cx, y - Off));
            sr.Add(DrawLine(X, y + Off + l + cy, X, y + Off + cy));
            sr.Add(DrawLine(X + cx, y + Off + l + cy, X + cx, y + Off + cy));
            AllShapes.Add(sr.Group());

            if (chkCu.Checked)
            {
                Cu = (double)nudCu.Value;
                sr.RemoveAll();
                sr.Add(DrawLine(X - Off - l, y + Cu, X - Off, y + Cu));
                sr.Add(DrawLine(X - Off - l, y + cy - Cu, X - Off, y + cy - Cu));
                sr.Add(DrawLine(X + cx + Off, y + Cu, X + cx + Off + l, y + Cu));
                sr.Add(DrawLine(X + cx + Off, y + cy - Cu, X + cx + Off + l, y + cy - Cu));
                sr.Add(DrawLine(X + Cu, y - Off - l, X + Cu, y - Off));
                sr.Add(DrawLine(X + cx - Cu, y - Off - l, X + cx - Cu, y - Off));
                sr.Add(DrawLine(X + Cu, y + Off + l + cy, X + Cu, y + Off + cy));
                sr.Add(DrawLine(X + cx - Cu, y + Off + l + cy, X + cx - Cu, y + Off + cy));
                AllShapes.Add(sr.Group());
            }

            if (chkZxsp.Checked)
            {
                sr.RemoveAll();
                sr.Add(DrawLine(X - Off - l, y + cy / 2, X, y + cy / 2));
                sr.Add(DrawLine(X + cx, y + cy / 2, X + cx + Off + l, y + cy / 2));
                sr.SetOutlinePropertiesEx(-1, new OutlineStyle(), application.CreateCMYKColor(100,100,100,100),null,null,cdrTriState.cdrFalse,cdrTriState.cdrFalse,cdrOutlineLineCaps.cdrOutlineButtLineCaps);
                //sr.SetOutlineProperties(0.2, new OutlineStyle(2));
                AllShapes.Add(sr.Group());
            }
            if (chkZxcz.Checked)
            {
                sr.RemoveAll();
                sr.Add(DrawLine(X + cx / 2, y + cy, X + cx / 2, y + cy + Off + l));
                sr.Add(DrawLine(X + cx / 2, y, X + cx / 2, y - Off - l));
                OutlineStyle ols = new OutlineStyle();
                sr.SetOutlineProperties(0.2, ols);

                AllShapes.Add(sr.Group());
            }
            if (chkJdsp.Checked)
            {
                Cu = (double)nudCu.Value;
                sr.RemoveAll();
                sr.Add(DrawLine(X - Off - l, y + cy / 2 + Cu, X - Off, y + cy / 2 + Cu));
                sr.Add(DrawLine(X - Off - l, y + cy / 2 - Cu, X - Off, y + cy / 2 - Cu));
                sr.Add(DrawLine(X + cx + Off, y + cy / 2 + Cu, X + cx + Off + l, y + cy / 2 + Cu));
                sr.Add(DrawLine(X + cx + Off, y + cy / 2 - Cu, X + cx + Off + l, y + cy / 2 - Cu));

                AllShapes.Add(sr.Group());
            }
            if (chkJdcz.Checked)
            {
                Cu = (double)nudCu.Value;
                sr.RemoveAll();
                sr.Add(DrawLine(X + cx / 2 + Cu, y + cy + Off, X + cx / 2 + Cu, y + cy + Off + l));
                sr.Add(DrawLine(X + cx / 2 - Cu, y + cy + Off, X + cx / 2 - Cu, y + cy + Off + l));
                sr.Add(DrawLine(X + cx / 2 + Cu, y - Off, X + cx / 2 + Cu, y - Off - l));
                sr.Add(DrawLine(X + cx / 2 - Cu, y - Off, X + cx / 2 - Cu, y - Off - l));

                AllShapes.Add(sr.Group());
            }

            AllShapes.Group();
            this.EndEvent();
        }
        private void btDo_Click(object sender, EventArgs e)
        {
            if (application.ActivePage.SelectableShapes.Count == 0)
                return;
            DoDrawCropMarks();
        }

        private void frmCropMark_Load(object sender, EventArgs e)
        {
            nudLen.Value = 2;
            nudOffset.Value = -1;
        }

        private void btClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}

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
    public partial class frmMakeHighlight : BaseForm
    {
        public frmMakeHighlight()
        {
            InitializeComponent();
        }
        public frmMakeHighlight(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.application = app;
            this.cmdName = "高光";
        }

        bool bul, bul2;
        double movX = 0, movY = 0;
        double X, Y;
        double mov, mov2;
        double w, h, offs;
        int kx, ky;
        int i;
        int cnt, DecSep;
        Shape s, s1, s2, s3;
        ShapeRange srorig, sr;

        private void Makelight(Shape s)
        {
            StartEvent(true);
            application.ActiveDocument.ReferencePoint = cdrReferencePoint.cdrBottomLeft;
            s = application.ActiveShape;
            offs = (double)numericUpDown1.Value;

            if (s.Fill.Type == cdrFillType.cdrNoFill)
            {
                s.Fill.ApplyUniformFill(application.CreateCMYKColor(0, 0, 0, 100));
            }
            sr = s.CreateContour(cdrContourDirection.cdrContourInside, offs, 1).Separate();
            sr.AddToSelection();
            s.RemoveFromSelection();
            s1 = application.ActiveShape;
            s1.Fill.ApplyUniformFill(application.CreateCMYKColor(0, 0, 0, 0));
            s2 = s1.Duplicate(0, 0);
            s2.GetPosition(out X, out Y);

            if (bul)
            {
                movX = X + mov * kx;
                movY = Y + mov * ky;
            }

            s2.OrderToFront();
            s2.SetPosition(movX, movY);
            s2.GetPosition(out X, out Y);

            s3 = s2.Trim(s1, true, false);
            s3.Outline.SetNoOutline();
            s2.Delete();
            EndEvent();
            s.CreateSelection();
        }
        private void frmMakeHighlight_Load(object sender, EventArgs e)
        {
            
            application.ActiveDocument.Unit = cdrUnit.cdrMillimeter;
            application.ActiveShape.GetSize(out w, out h);
            offs = Math.Round(h / 50, 2);
            bul = false;
            numericUpDown1.Value = (decimal)offs;
            mov = offs / 2;
            srorig = application.ActiveSelectionRange;
            cnt = application.ActiveSelection.Shapes.Count;

            application.ActiveSelectionRange.Copy();

            if (cnt > 1)
            {
                application.ActiveSelectionRange.ConvertToCurves();
                s = application.ActiveSelection.Combine();
            }
            else
            {
                application.ActiveSelectionRange.ConvertToCurves();
                s = application.ActiveShape;
            }
            kx = 0;
            ky = -1;
            Makelight(s);
            bul = true;
            s.CreateSelection();
            application.ActiveWindow.Refresh();
        }
        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            if (bul)
            {
                application.ActiveDocument.Undo(1);
                s.CreateSelection();
                kx = 0;
                ky = 0;
                movX = X + mov * kx;
                movY = Y + mov * ky;
                Makelight(s);
                s.CreateSelection();
            }
            application.ActiveWindow.Refresh();
        }

        private void button_left_Click(object sender, EventArgs e)
        {
            application.ActiveDocument.Undo(1);
            s.CreateSelection();
            kx = -1;
            ky = 0;
            movX = X + mov * kx;
            movY = Y + mov * ky;
            Makelight(s);
            s.CreateSelection();
            application.ActiveWindow.Refresh();
        }

        private void button_right_Click(object sender, EventArgs e)
        {
            application.ActiveDocument.Undo(1);
            s.CreateSelection();
            kx = 1;
            ky = 0;
            movX = X + mov * kx;
            movY = Y + mov * ky;
            Makelight(s);
            s.CreateSelection();
            application.ActiveWindow.Refresh();
        }

        private void button_up_Click(object sender, EventArgs e)
        {
            application.ActiveDocument.Undo(1);
            s.CreateSelection();
            kx = 0;
            ky = 1;
            movX = X + mov * kx;
            movY = Y + mov * ky;
            Makelight(s);
            s.CreateSelection();
            application.ActiveWindow.Refresh();
        }

        private void button_down_Click(object sender, EventArgs e)
        {
            application.ActiveDocument.Undo(1);
            s.CreateSelection();
            kx = 0;
            ky = -1;
            movX = X + mov * kx;
            movY = Y + mov * ky;
            Makelight(s);
            s.CreateSelection();
            application.ActiveWindow.Refresh();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            bul2 = true;
            s.Delete();
            application.ActivePage.ActiveLayer.Paste().OrderBackOf(s3);
            s3.OrderToFront();
            application.Clipboard.Clear();
            Close();
        }
    }

}

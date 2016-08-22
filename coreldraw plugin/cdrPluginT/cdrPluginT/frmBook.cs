using Corel.Interop.VGCore;
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
    public partial class frmBook : BaseForm
    {
        public frmBook()
        {
            InitializeComponent();
        }

        public frmBook(Corel.Interop.VGCore.Application app)
            : base(app)
        {
            InitializeComponent();
        }

        private void frmBook_Load(object sender, EventArgs e)
        {
            comboBox1.SelectedIndex = 0;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            StartEvent(checkBox1.Checked);
            foreach (Page p in application.ActiveDocument.Pages)
            {
                doPage(p);
            }
            EndEvent();
        }

        private void doPage(Page page)
        {
            page.Activate();
            double w = page.SizeWidth;
            double h = page.SizeHeight;
            page.SelectShapesFromRectangle(0, 0, w, h, true).Group();
            ShapeRange shapeRange = application.ActiveSelectionRange;
            ShapeRange shapeRangeD = shapeRange.Duplicate();

            Shape rec = null, recD = null;
            if (comboBox1.SelectedIndex == 0)
            {
                rec = application.ActiveLayer.CreateRectangle2(0, h / 2, w, h / 2);
                recD = application.ActiveLayer.CreateRectangle2(0, 0, w, h / 2);
            }
            if (comboBox1.SelectedIndex == 1)
            {
                rec = application.ActiveLayer.CreateRectangle2(0, 0, w / 2, h);
                recD = application.ActiveLayer.CreateRectangle2(w / 2, 0, w / 2, h);
            }
            rec.Fill.ApplyNoFill();
            rec.Outline.Width = 0;
            recD.Fill.ApplyNoFill();
            recD.Outline.Width = 0;
            shapeRange.AddToPowerClip(rec, cdrTriState.cdrFalse);
            shapeRangeD.AddToPowerClip(recD, cdrTriState.cdrFalse);
            rec.OrderToBack();
            recD.OrderToBack();

            double newW, newH;
            if (comboBox1.SelectedIndex == 0)
            {
                newW = w;
                newH = h / 2;
            }
            else
            {
                newW = w / 2;
                newH = h;
            }

            page.SizeWidth = newW;
            page.SizeHeight = newH;
            rec.LeftX = 0;
            rec.BottomY = 0;

            Page newPage = application.ActiveDocument.AddPagesEx(1, newW, newH);
            newPage.Activate();
            recD.MoveToLayer(application.ActiveLayer);
            recD.LeftX = 0;
            recD.BottomY = 0;

            newPage.MoveTo(page.Index + 1);


            shapeRange.Ungroup();
            shapeRangeD.Ungroup();
        }
    }
}

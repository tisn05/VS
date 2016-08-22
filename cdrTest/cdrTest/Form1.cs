using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Corel.Interop.VGCore;

namespace cdrTest
{
    public partial class Form1 : BaseForm
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            frmToJPG ftj = new frmToJPG();
            ftj.Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            frmConvertTo f = new frmConvertTo();
            f.Show();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            frmCropMark f = new frmCropMark();
            f.Show();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            frmGuideLine f = new frmGuideLine();
            f.Show();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button5_Click(object sender, EventArgs e)
        {
            frmDelViews f = new frmDelViews();
            f.Show();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            frmQrCode f = new frmQrCode();
            f.Show();
        }

        private void button7_Click(object sender, EventArgs e)
        {
            frmDelSymbols f = new frmDelSymbols();
            f.Show();
        }

        private void button8_Click(object sender, EventArgs e)
        {
            frmPStoCurve f = new frmPStoCurve();
            f.Show();
        }

        private void button9_Click(object sender, EventArgs e)
        {
            frmColorReplace f = new frmColorReplace();
            f.Show();
        }

        private void button10_Click(object sender, EventArgs e)
        {
            Shape sh;
            foreach(Page p in app.ActiveDocument.Pages){
                sh = p.SelectShapesFromRectangle(0, 0, p.SizeWidth, p.SizeHeight, true);
                sh.Group();
            }
        }

        private void button11_Click(object sender, EventArgs e)
        {
            frmImport f = new frmImport();
            f.Show();
        }

        private void button12_Click(object sender, EventArgs e)
        {
            frmBook f = new frmBook();
            f.Show();
        }

        private void button13_Click(object sender, EventArgs e)
        {
            frmCompressPic f = new frmCompressPic();
            f.Show();
        }
    }
}

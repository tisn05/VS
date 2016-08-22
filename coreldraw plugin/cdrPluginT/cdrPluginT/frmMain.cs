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
    public partial class frmMain : BaseForm
    {
        public frmMain()
        {
            InitializeComponent();
        }

        public frmMain(Corel.Interop.VGCore.Application app)
            : base(app)
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            frmToJPG ftj = new frmToJPG(this.application);
            ftj.Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            frmConvertTo f = new frmConvertTo(this.application);
            f.Show();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            frmCropMark f = new frmCropMark(this.application);
            f.Show();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            frmGuideLine f = new frmGuideLine(this.application);
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
            frmQrCode f = new frmQrCode(this.application);
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
            frmColorReplace f = new frmColorReplace(this.application);
            f.Show();
        }

        private void button10_Click(object sender, EventArgs e)
        {
            Shape sh;
            foreach (Page p in application.ActiveDocument.Pages)
            {
                sh = p.SelectShapesFromRectangle(0, 0, p.SizeWidth, p.SizeHeight, true);
                sh.Group();
            }
        }

        private void button11_Click(object sender, EventArgs e)
        {
            frmImport f = new frmImport(this.application);
            f.Show();
        }

        private void button12_Click(object sender, EventArgs e)
        {
            frmBook f = new frmBook(this.application);
            f.Show();
        }

        private void button13_Click(object sender, EventArgs e)
        {
            frmCompressPic f = new frmCompressPic(this.application);
            f.Show();
        }

        private void button15_Click(object sender, EventArgs e)
        {
            frmSelect f = new frmSelect(this.application);
            f.Show();
        }

        private void button16_Click(object sender, EventArgs e)
        {
            frmAlign f = new frmAlign(this.application);
            f.Show();
        }

        private void button17_Click(object sender, EventArgs e)
        {
            frmPicIn f = new frmPicIn(this.application);
            f.Show();
        }

        private void button18_Click(object sender, EventArgs e)
        {
            frmShowHide f = new frmShowHide(this.application);
            f.Show();
        }

        private void button19_Click(object sender, EventArgs e)
        {
            frmAIClipBoard f = new frmAIClipBoard(this.application);
            f.Show();
        }

        private void button20_Click(object sender, EventArgs e)
        {
            frmRemoveViews f = new frmRemoveViews(this.application);
            f.Show();
        }

        bool sq = false;
        private void button14_Click(object sender, EventArgs e)
        {
            if (!sq)
            {
                button14.Text = "展开";
                this.Height = 75;
                sq = true;
            }
            else
            {
                button14.Text = "收起";
                this.Height = 287;
                sq = false;
            }
        }

        private void button21_Click(object sender, EventArgs e)
        {
            frmMakeHighlight f = new frmMakeHighlight(this.application);
            f.Show();
        }

        private void button22_Click(object sender, EventArgs e)
        {
            frmScreen f = new frmScreen(this.application);
            f.Show();
        }

        private void button23_Click(object sender, EventArgs e)
        {
            frmFindFont f = new frmFindFont(this.application);
            f.Show();
        }

        private void button24_Click(object sender, EventArgs e)
        {
            frmTest f = new frmTest(this.application);
            f.Show();
        }
    }
}

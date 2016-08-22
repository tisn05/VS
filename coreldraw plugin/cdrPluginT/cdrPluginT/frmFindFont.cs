using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Corel.Interop.VGCore;
using Bitmap = System.Drawing.Bitmap;

namespace cdrPluginT
{
    public partial class frmFindFont : BaseForm
    {
        public frmFindFont()
        {
            InitializeComponent();
        }

        public frmFindFont(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.application = app;
        }

        private void frmFindFont_Load(object sender, EventArgs e)
        {
            foreach (String f in application.FontList)
            {
                comboBox1.Items.Add(f);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Bitmap bmp = new Bitmap(250, 20);
            Graphics gra = Graphics.FromImage(bmp);
            gra.Clear(System.Drawing.Color.White);
            gra.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            Font ft = new Font(comboBox1.Text,12, FontStyle.Regular);
            Brush brush = new SolidBrush(System.Drawing.Color.Black);
            gra.DrawString(comboBox1.Text, ft, brush, new PointF(0, 0));
            imageList1.Images.Add(bmp);
            listView1.Items.Clear();
            for (int i = 0; i < imageList1.Images.Count; i++)
            {
                ListViewItem lvi = new ListViewItem();
                lvi.ImageIndex = i;
                this.listView1.Items.Add(lvi);
            }
            dataGridView1.Rows.Add(new object[] {bmp, bmp });
        }
    }
}

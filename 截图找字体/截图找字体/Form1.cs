using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace 截图找字体
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

            Bitmap bmp = new Bitmap(100, 20);
            Graphics gra = Graphics.FromImage(bmp);
            gra.Clear(System.Drawing.Color.White);
            Font ft = new Font("宋体", 12, FontStyle.Regular);
            Brush brush = new SolidBrush(System.Drawing.Color.Red);
            gra.DrawString("宋体", ft, brush, new PointF(0, 0));
            imageList1.Images.Add(bmp);
            pictureBox1.Image = bmp;
            listView1.Items.Clear();
            for (int i = 0; i < imageList1.Images.Count; i++)
            {
                ListViewItem lvi = new ListViewItem();
                lvi.ImageIndex = i;
                this.listView1.Items.Add(lvi);
            }

            dataGridView1.Rows.Add(new object[] { bmp });
        }
    }
}

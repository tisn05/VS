using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using ICSharpCode.SharpZipLib.Zip;
using tisn.cdr;

namespace WindowsApplication1
{
    public partial class frmMain : Form
    {
        public frmMain()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
        }

        private void frmMain_Load(object sender, EventArgs e)
        {

        }

        private void listBox1_DragDrop(object sender, DragEventArgs e)
        {
            String name = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            backgroundWorker1.RunWorkerAsync(name);
        }

        private void listBox1_DragOver(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.All;
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            String riffFile = String.Empty;
            String name = (String)e.Argument;
            bool m14 = false;
            if (!File.Exists(name))
            {
                return;
            }
            FileStream fs = File.OpenRead(name);
            BinaryReader br = new BinaryReader(fs);
            byte[] hd = new byte[2];
            br.Read(hd, 0, 2);
            if (Encoding.UTF8.GetString(hd) == "RI")
            {
                riffFile = name;
            }
            else if (Encoding.UTF8.GetString(hd) == "PK")
            {
                m14 = true;
                ZipInputStream zis = new ZipInputStream(File.OpenRead(name));
                ZipEntry theEntry;
                bool d = false;
                while ((theEntry = zis.GetNextEntry()) != null)
                {
                    String fileName;
                    if (theEntry.Name == "content/riffData.cdr" || theEntry.Name == "content/root.dat")
                    {
                        fileName = Path.GetFileName(theEntry.Name);
                        String tmp = Path.GetTempPath();
                        riffFile = tmp + fileName;
                        FileStream fs1 = File.Open(tmp + fileName, FileMode.OpenOrCreate);
                        BinaryWriter bw = new BinaryWriter(fs1);
                        int size = 2048;
                        byte[] data = new byte[2048];
                        while (true)
                        {
                            size = zis.Read(data, 0, data.Length);
                            if (size > 0)
                            {
                                bw.Write(data, 0, size);
                            }
                            else
                            {
                                break;
                            }
                        }
                        bw.Close();
                        fs1.Close();
                        zis.Close();
                        d = true;
                        break;
                    }
                }
                if (!d)
                {
                    MessageBox.Show("文件格式错误！");
                    return;
                }
            }
            else
            {
                MessageBox.Show("文件格式错误！");
                return;
            }
            br.Close();
            fs.Close();
            if (m14)
            {

            }
            RiffParser rp = new RiffParser();
            rp.OpenFile(riffFile);
            CDRReader cr = new CDRReader(rp);
            cr.Read();
            /*CDRWriter cw = new CDRWriter();
            CDRFile cf = cr.CDRFile;
            cf.list = RiffConn.DelRiff<CDRbmkt>(cf.list);
            //cf.list = RiffConn.DelRiff<CDRcolo>(cf.list);
            cf.list = RiffConn.DelRiff<CDRsyst>(cf.list);
            cw.SetCDRFile(cf);
            cw.Write(name + "_1");*/
            listBox1.Items.Clear();
            listBox1.Items.Add("文件名：");
            listBox1.Items.Add("    " + Path.GetFileName(name));
            listBox1.Items.Add("文件版本：");
            listBox1.Items.Add(String.Format("    CorelDraw {0:d}", cr.CDRFile.Version));
            listBox1.Items.Add("保存版本：");
            listBox1.Items.Add(String.Format("    CorelDraw {0:d} 子版本：{1:d}", cr.CDRFile.SVersion, cr.CDRFile.BuildID));
            listBox1.Items.Add("位图数量：");
            listBox1.Items.Add(String.Format("    {0:d}", cr.CDRFile.BMPCount));
            listBox1.Items.Add("视图样式数量：");
            listBox1.Items.Add(String.Format("    {0:d}", cr.CDRFile.ViewCount));
            listBox1.Items.Add(String.Format("使用字体：数量：{0:d}", cr.CDRFile.fonts.Count));
            foreach (CDRFile.Fonts f in cr.CDRFile.fonts)
            {
                listBox1.Items.Add(String.Format("    {0:1}", f.name));
            }
            rp.CloseFile();
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}

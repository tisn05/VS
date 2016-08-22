using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace UC小说转换
{
    public class UCNovel : FileStream
    {
        private Dictionary<String, int[]> mList;
        public UCNovel(String path)
            : base(path, FileMode.Open)
        {
            mList = new Dictionary<string, int[]>();
            this.Seek(0, SeekOrigin.Begin);
            byte[] tmp = new byte[4];
            this.Read(tmp, 0, 4);
            if (tmp != new byte[] { 0x0f, 0, 0, 0 })
            {
                //throw new Exception();
            }
            tmp = new byte[15];
            this.Read(tmp, 0, 15);
            if (Encoding.Default.GetString(tmp) != "novel_data_v1.0")
            {
                throw new Exception();
            }
            GetList();
        }

        public void GetList()
        {
            this.Seek(19, SeekOrigin.Begin);
            this.Position += 4;
            int f = -1;
            byte[] l = new byte[4];
            byte[] tmp;
            String name;
            int[] pos = new int[2];
            int ll = 0;
            while (true)
            {
                this.Position += 4;
                this.Read(l, 0, 4);
                ll = BitConverter.ToInt32(l, 0);
                tmp = new byte[ll];
                this.Read(tmp, 0, ll);
                name = Encoding.UTF8.GetString(tmp);
                this.Read(l, 0, 4);

                pos[0] = BitConverter.ToInt32(l, 0);
                if (f == -1)
                {
                    f = pos[0];
                }
                this.Read(l, 0, 4);
                pos[1] = BitConverter.ToInt32(l, 0);
                mList.Add(name, pos);
                if (pos[1] == this.Length - 1)
                {
                    break;
                }
            }
            return;

        }
        public String GetContent(String content_key)
        {
            int[] pos = mList[content_key];
            this.Seek(pos[0], SeekOrigin.Begin);
            int len = pos[1] - pos[0];
            byte[] tmp = new byte[len];
            this.Read(tmp, 0, len);
            String content = Encoding.UTF8.GetString(tmp);
            return content;
        }
    }
}

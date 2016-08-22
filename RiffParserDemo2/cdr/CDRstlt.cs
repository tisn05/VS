using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace tisn.cdr
{
    public class CDRstlt : RiffList
    {
        public CDRstlt(CDRFile p) : base(p) { }

        public override void Read(System.IO.BinaryReader br, int len, int[] blockSizes = null)
        {
            if (len > 4)
            {
                long p = br.BaseStream.Position;
                //br.ReadInt32();
                int d1 = br.ReadInt32();
                Debug.WriteLine(String.Format("Num of style entries {0:d}", d1));
                int d2 = br.ReadInt32();
                Debug.WriteLine(String.Format("fild list {0:d}", d2));
                for (int i = 0; i < d2; i++)
                {
                    int t1, t2, t3;
                    t1 = br.ReadInt32();
                    t2 = br.ReadInt32();
                    t3 = br.ReadInt32();
                    if (Parent.Version > 12)
                    {
                        byte[] tb = br.ReadBytes(48);
                    }
                }
                d2 = br.ReadInt32();
                Debug.WriteLine(String.Format("outl list {0:d}", d2));
                for (int i = 0; i < d2; i++)
                {
                    int t1, t2, t3;
                    t1 = br.ReadInt32();
                    t2 = br.ReadInt32();
                    t3 = br.ReadInt32();
                }
                d2 = br.ReadInt32();
                Debug.WriteLine(String.Format("font list {0:d}", d2));
                int size = 56;
                if (Parent.Version < 10)
                    size = 40;
                for (int i = 0; i < d2; i++)
                {
                    int IDs = br.ReadInt32();
                    Debug.WriteLine(String.Format("    ID: {0:d}", IDs));
                    br.ReadBytes(size);
                }
                d2 = br.ReadInt32();
                Debug.WriteLine(String.Format("align list {0:d}", d2));
                for (int i = 0; i < d2; i++)
                {
                    int IDs = br.ReadInt32();
                    Debug.WriteLine(String.Format("    ID: {0:d}", IDs));
                    br.ReadInt64();
                }


                br.BaseStream.Position = p;
            }
            this.postoin = br.BaseStream.Position - 8;
            this.length = len;
            br.BaseStream.Position += len;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace tisn.cdr
{
    /// <summary>
    /// 填充
    /// </summary>
    public class CDRfild : RiffChunk
    {
        public CDRfild(CDRFile p)
            : base(p)
        {
            Parent.FillCout++;
        }

        public override void Read(System.IO.BinaryReader br, int len, int[] blockSizes = null)
        {
            long p = br.BaseStream.Position;

            int FillID = br.ReadInt32();
            byte fill_type;

            if (Parent.Version > 12)
                br.ReadInt64();
            fill_type = br.ReadByte();


            String ft_txt = String.Format("{0:d}", fill_type);
            if (CDRFile.Fill_Type.ContainsKey(fill_type))
            {
                ft_txt = CDRFile.Fill_Type[fill_type];
            }


            br.BaseStream.Position = p;
            base.Read(br, len, blockSizes);
        }
    }
}

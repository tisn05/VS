using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using ComponentAce.Compression.Libs.zlib;

namespace tisn.cdr
{
    public class CDRcmpr : RiffList
    {
        public CDRcmpr(CDRFile p) : base(p) { }
        public override void Read(BinaryReader br, int len, int[] bs = null)
        {
            if (this.Type != RiffType.LIST)
            {
                base.Read(br, len);
            }
            int zdataLen, dataLen, zblkLen, blocksLen4;
            zdataLen = br.ReadInt32();
            dataLen = br.ReadInt32();
            zblkLen = br.ReadInt32();
            blocksLen4 = br.ReadInt32();
            br.ReadInt64();
            byte[] zdata = new byte[zdataLen - 8];
            byte[] zblk = new byte[zblkLen - 8];
            br.Read(zdata, 0, zdataLen - 8);
            br.ReadInt64();
            br.Read(zblk, 0, zblkLen - 8);
            byte[] data = Zlib.deCompressBytes(zdata);
            byte[] blocks = Zlib.deCompressBytes(zblk);
            //byte[] data = ZLibNet.ZLibCompressor.DeCompress(zdata);
            //byte[] blocks = ZLibNet.ZLibCompressor.DeCompress(zblk);
            FileStream fs = File.Open("C:\\blocks", FileMode.OpenOrCreate | FileMode.Truncate);
            fs.Write(blocks, 0, blocks.Length);
            fs.Close();
            fs = File.Open("C:\\data", FileMode.OpenOrCreate | FileMode.Truncate);
            fs.Write(data, 0, data.Length);
            fs.Close();
            int[] blockSizes = new int[blocks.Length / 4];
            for (int i = 0, j = 0; i < blocks.Length; i += 4, j++)
            {
                blockSizes[j] = BitConverter.ToInt32(blocks, i);
            }
            MemoryStream ms = new MemoryStream(data);
            BinaryReader br1 = new BinaryReader(ms);
            subList = RiffConn.ReadRiff(this.Parent, br1, blockSizes);
            br1.Close();
            ms.Close();
        }
        public override int Write(BinaryWriter bw)
        {
            long sp = bw.BaseStream.Position;
            int[] blockSizes = new int[0];
            byte[] data = RiffConn.WriteRiff(subList, ref blockSizes, true);
            FileStream fs = File.Open("C:\\data1", FileMode.OpenOrCreate | FileMode.Truncate);
            fs.Write(data, 0, data.Length);
            fs.Close();
            byte[] zdata = ZLibNet.ZLibCompressor.Compress(data);
            byte[] blocks = new byte[blockSizes.Length * 4];
            MemoryStream ms = new MemoryStream(blocks);
            BinaryWriter b2 = new BinaryWriter(ms);
            for (int i = 0, j = 0; i < blocks.Length; i += 4, j++)
            {
                b2.Write(blockSizes[j]);
            }
            fs = File.Open("C:\\blocks1", FileMode.OpenOrCreate | FileMode.Truncate);
            fs.Write(blocks, 0, blocks.Length);
            fs.Close();
            b2.Close();
            ms.Close();
            byte[] zblk = ZLibNet.ZLibCompressor.Compress(blocks);
            byte[] wt;
            ms = new MemoryStream();
            ms.SetLength(99999999);
            BinaryWriter bww = new BinaryWriter(ms);
            bww.Write(RiffParser.ToFourCC("cmpr"));
            bww.Write(zdata.Length + 8);
            bww.Write(data.Length);
            bww.Write(zblk.Length + 8);
            bww.Write(blocks.Length);
            bww.Write(Encoding.UTF8.GetBytes("CPng\x01\x00\x04\x00"));
            bww.Write(zdata);
            bww.Write(Encoding.UTF8.GetBytes("CPng\x01\x00\x04\x00"));
            bww.Write(zblk);
            int p = (int)ms.Position;
            wt = new byte[p];
            Array.Copy(ms.GetBuffer(), wt, p);
            bww.Close();
            ms.Close();
            bw.Write(RiffParser.ToFourCC("LIST"));
            bw.Write(wt.Length);
            bw.Write(wt, 0, p);
            int len = (int)(bw.BaseStream.Position - sp);
            if ((len & 1) != 0)
            {
                bw.Write(false);
            }
            return len;
        }
    }
}

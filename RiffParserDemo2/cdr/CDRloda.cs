using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace tisn.cdr
{
    public class CDRloda : RiffChunk
    {
        public CDRloda(CDRFile p) : base(p) { }

        public override void Read(BinaryReader br, int len, int[] blockSizes = null)
        {
            long p = br.BaseStream.Position;

            br.ReadInt32();
            int numofparms = br.ReadInt32();
            int startofparms = br.ReadInt32();
            int startoftypes = br.ReadInt32();

            byte type = br.ReadByte();
            for (int i = 0; i < numofparms; i++)
            {
                br.BaseStream.Position = p + (startofparms + (i * 4));
                int offset = br.ReadInt32();
                int length = br.ReadInt32() - offset;
                br.BaseStream.Position = p + (startoftypes + ((numofparms - 1 - i) * 4));
                int argtype = br.ReadInt32();
                switch (argtype)
                {
                    case 0xa://loda_outl
                        break;
                    case 0x14://loda_fild
                        break;
                    case 0x1e://loda_coords
                        loda_coords(br, type, p + offset, length);
                        break;
                    case 0x28://loda_rot_center
                        break;
                    case 0x64://loda_trfd
                        break;
                    case 0xc8://loda_stlt
                        break;
                    case 0x3e8://loda_name
                        loda_name(br, type, p + offset, length);
                        break;
                    case 0x7d0://loda_palt
                        break;
                    case 0x1f40://loda_lens
                        break;
                    case 0x1f45://loda_contnr
                        break;
                    case 0x2af8://loda_polygon
                        break;
                    case 0x2eea://loda_grad
                        break;
                    case 0x2efe://loda_rot
                        break;
                    case 0x32c8://loda_wroff
                        break;
                    case 0x32c9://loda_wrstyle
                        break;
                    case 0x4ace://loda_mesh
                        break;
                    default:
                        break;
                }
            }

            switch (type)
            {
                case 0://图层
                    break;
                case 1://矩形
                    Parent.Rectangles++;
                    break;
                case 2://椭圆形
                    Parent.Ellipses++;
                    break;
                case 3://曲线
                    Parent.Curves++;
                    break;
                case 4://艺术字
                    Parent.ArtisticTexts++;
                    break;
                case 5://位图
                    Parent.BMPCount++;
                    break;
                case 6://段落文本
                    Parent.ParagraphText++;
                    break;
                case 0xa:
                    break;
                case 0xb:
                    break;
                case 0xc://辅助线
                    break;
                case 0x10:
                    break;
                case 0x11://桌面
                    break;
                case 0x14://多边形
                    Parent.Polygons++;
                    break;
                case 0x20://网格
                    break;
                case 0x25://完美形状
                    break;
                case 0x26:
                    break;
                case 0x27://尺度
                    break;
                default:
                    break;
            }



            br.BaseStream.Position = p;
            base.Read(br, len, blockSizes);
        }

        void loda_outl(BinaryReader br, byte type, long offset, int len)
        {

        }

        void loda_coords(BinaryReader br, byte type, long offset, int len)
        {

        }

        void loda_fild(BinaryReader br, byte type, long offset, int len)
        {

        }

        void loda_name(BinaryReader br, byte type, long offset, int len)
        {
            String layrname;
            br.BaseStream.Position = offset;
            byte[] tm = br.ReadBytes(len);
            if (Parent.Version > 11)
            {
                layrname = Encoding.Unicode.GetString(tm);
            }
            else
                layrname = Encoding.Default.GetString(tm);
        }
    }
}

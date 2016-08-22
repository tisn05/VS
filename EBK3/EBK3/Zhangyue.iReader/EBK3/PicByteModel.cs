namespace Zhangyue.iReader.EBK3
{
    using System;

    public class PicByteModel
    {
        public byte[] bytecover_data;
        public byte[] bytecover_data_length = new byte[4];
        public byte[] bytecover_height = new byte[4];
        public byte[] bytecover_img_type = new byte[4];
        public byte[] bytecover_width = new byte[4];
    }
}


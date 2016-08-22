namespace Zhangyue.iReader.EBK3
{
    using System;

    public class Pic
    {
        public byte[] GetPicByte(PicModel model)
        {
            PicByteModel model2 = new PicByteModel();
            object[] objArray = new object[5];
            model2.bytecover_img_type = BitConverter.GetBytes(model.cover_img_type);
            model2.bytecover_width = BitConverter.GetBytes(model.cover_width);
            model2.bytecover_height = BitConverter.GetBytes(model.cover_height);
            model2.bytecover_data_length = BitConverter.GetBytes(model.cover_data_length);
            model2.bytecover_data = model.cover_data;
            objArray[0] = model2.bytecover_data_length;
            objArray[1] = model2.bytecover_img_type;
            objArray[2] = model2.bytecover_width;
            objArray[3] = model2.bytecover_height;
            objArray[4] = model2.bytecover_data = model.cover_data;
            int num = (((model2.bytecover_img_type.Length + model2.bytecover_width.Length) + model2.bytecover_height.Length) + model2.bytecover_data_length.Length) + model2.bytecover_data.Length;
            byte[] buffer = new byte[num];
            int num2 = 0;
            for (int i = 0; i < objArray.Length; i++)
            {
                byte[] buffer2 = objArray[i] as byte[];
                Array.Copy(buffer2, 0, buffer, num2, buffer2.Length);
                num2 += buffer2.Length;
            }
            return buffer;
        }
    }
}


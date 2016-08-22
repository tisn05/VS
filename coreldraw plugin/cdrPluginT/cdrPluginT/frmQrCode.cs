using CSharpWin;
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.IO;
using ZXing;

using Bitmap = System.Drawing.Bitmap;
using ZXing.QrCode;
using ErrorCorrectionLevel = ZXing.QrCode.Internal.ErrorCorrectionLevel;
using ZXing.Rendering;
using Version = ZXing.QrCode.Internal.Version;
using ThoughtWorks.QRCode.Codec;

namespace cdrPluginT
{
    public partial class frmQrCode : BaseForm
    {
        private String qCode = "";
        BarcodeWriter qrCodeWriter = null;
        QrCodeEncodingOptions options = null;
        QRCodeEncoder qrCodeEncoder;

        public frmQrCode()
        {
            InitializeComponent();
        }

        public frmQrCode(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.application = app;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
        }

        private void frmQrCode_Load(object sender, EventArgs e)
        {
            qrCodeEncoder = new QRCodeEncoder();
            qrCodeEncoder.QRCodeErrorCorrect = QRCodeEncoder.ERROR_CORRECTION.L;
            qrCodeEncoder.QRCodeVersion = 0;
            qrCodeEncoder.QRCodeScale = 4;

            qrCodeWriter = new BarcodeWriter();
            qrCodeWriter.Format = BarcodeFormat.QR_CODE;
            options = new QrCodeEncodingOptions();
            options.ErrorCorrection = ErrorCorrectionLevel.M;
            //options.CharacterSet = "utf-8";
            options.Width = pictureBox1.Width;
            options.Height = pictureBox1.Height;
            qrCodeWriter.Options = options;
            comboBox_ErrorCorrect.SelectedIndex = ErrorCorrectionLevel.M.Bits;
            comboBox_version.SelectedIndex = 0;

        }

        private void button1_Click(object sender, EventArgs e)
        {
        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    refreshIDCard();
                    break;
                case 1:
                    break;
                case 2:
                    refreshSelfText();
                    break;
            }
        }

        private void loadQrSetting()
        {

        }

        private void refreshSelfText()
        {
            qCode = textBox_selfText.Text;
            refreshQrCode();
        }

        private void refreshIDCard()
        {
            qCode = "BEGIN:VCARD \r\n";
            bool d = false;
            if (textBox_cardName.Text != "")
            {
                qCode += "FN:" + textBox_cardName.Text + "\r\n";
                d = true;
            }
            if (textBox_cardPosition.Text != "")
            {
                qCode += "TITLE:" + textBox_cardPosition.Text + "\r\n";
                d = true;
            }
            if (textBox_cardHttp.Text != "")
            {
                qCode += "URL:" + textBox_cardHttp.Text + "\r\n";
                d = true;
            }
            if (textBox_cardTel.Text != "")
            {
                qCode += "TEL;CELL:" + textBox_cardTel.Text + "\r\n";
                d = true;
            }
            if (textBox_cardQQ.Text != "")
            {
                qCode += "X-QQ:" + textBox_cardQQ.Text + "\r\n";
                d = true;
            }
            if (textBox_cardEMail.Text != "")
            {
                qCode += "EMAIL:" + textBox_cardEMail.Text + "\r\n";
                d = true;
            }
            if (textBox_cardCompany.Text != "")
            {
                qCode += "ORG:" + textBox_cardCompany.Text + "\r\n";
                d = true;
            }
            if (textBox_cardAdress.Text != "")
            {
                qCode += "ADR;WORK:" + textBox_cardAdress.Text + "\r\n";
                d = true;
            }
            qCode += "END:VCARD";
            if (!d)
            {
                qCode = "";
            }
            textBox_cardReal.Text = qCode;
            refreshQrCode();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            CaptureImageTool cit = new CaptureImageTool();
            if (cit.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                try
                {
                    BarcodeReader reader = new BarcodeReader();
                    List<BarcodeFormat> li = new List<BarcodeFormat>();
                    li.Add(BarcodeFormat.QR_CODE);
                    reader.Options.PossibleFormats = li;
                    Result result = reader.Decode((Bitmap)cit.Image);
                    String errcor = (String)result.ResultMetadata[ResultMetadataType.ERROR_CORRECTION_LEVEL];
                    if (errcor == "L")
                    {
                        comboBox_ErrorCorrect.SelectedIndex = 0;
                    }
                    else if (errcor == "M")
                    {
                        comboBox_ErrorCorrect.SelectedIndex = 1;
                    }
                    else if (errcor == "H")
                    {
                        comboBox_ErrorCorrect.SelectedIndex = 2;
                    }
                    else if (errcor == "Q")
                    {
                        comboBox_ErrorCorrect.SelectedIndex = 3;
                    }
                    String decodedString = result.Text;
                    textBox_selfText.Text = decodedString;
                    refreshSelfText();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("无法识别的二维码！");
                }
            }
        }

        private void textBox_selfText_TextChanged(object sender, EventArgs e)
        {
            refreshSelfText();
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            refreshIDCard();
        }

        private void textBox_cardPosition_TextChanged(object sender, EventArgs e)
        {
            refreshIDCard();
        }

        private void textBox_cardHttp_TextChanged(object sender, EventArgs e)
        {
            refreshIDCard();
        }

        private void textBox_cardTel_TextChanged(object sender, EventArgs e)
        {
            refreshIDCard();
        }

        private void textBox_cardQQ_TextChanged(object sender, EventArgs e)
        {
            refreshIDCard();
        }

        private void textBox_cardEMail_TextChanged(object sender, EventArgs e)
        {
            refreshIDCard();
        }

        private void textBox_cardCompany_TextChanged(object sender, EventArgs e)
        {
            refreshIDCard();
        }

        private void textBox_cardAdress_TextChanged(object sender, EventArgs e)
        {
            refreshIDCard();
        }

        private bool[,] t2t(bool[][] b)
        {
            bool[,] ob = new bool[b.Length, b.Length];
            for (int i = 0; i < b.Length; i++)
            {
                for (int j = 0; j < b.Length; j++)
                {
                    ob[i, j] = b[i][j];
                }
            }
            return ob;
        }

        private void button_toVector_Click(object sender, EventArgs e)
        {
            StartEvent();
            SvgRenderer svg = new SvgRenderer();
            SvgRenderer.SvgImage si = svg.Render(qrCodeWriter.Encode(qCode), qrCodeWriter.Format, qCode);
            var file = File.Open("D:\\a.eps", FileMode.OpenOrCreate);
            byte[] b = Encoding.UTF8.GetBytes(si.ToString());
            file.Write(b, 0, b.Length);
            file.Close();
            application.ActiveLayer.Import("D:\\a.eps");
            //File.Delete("D:\\a.eps");
            EndEvent();
        }

        private void refreshQrCode()
        {
            if (qCode == "")
                return;
            //pictureBox1.Image = qrCodeWriter.Write(qCode);
            qrCodeEncoder.QRCodeVersion = 0;
            pictureBox1.Image = qrCodeEncoder.Encode(qCode, Encoding.UTF8);
        }

        private void comboBox_ErrorCorrect_SelectedIndexChanged(object sender, EventArgs e)
        {
            QrCodeEncodingOptions qceo = (QrCodeEncodingOptions)qrCodeWriter.Options;
            qceo.ErrorCorrection = ErrorCorrectionLevel.forBits(comboBox_ErrorCorrect.SelectedIndex);
            qrCodeEncoder.QRCodeErrorCorrect = (ThoughtWorks.QRCode.Codec.QRCodeEncoder.ERROR_CORRECTION)qceo.ErrorCorrection.Bits;
            refreshQrCode();
        }

        private void textBox_cardReal_TextChanged(object sender, EventArgs e)
        {

        }

        private void comboBox_version_SelectedIndexChanged(object sender, EventArgs e)
        {
            /*if (comboBox_version.SelectedIndex == 0)
                ((QrCodeEncodingOptions)qrCodeWriter.Options).Version = null;
            else
                ((QrCodeEncodingOptions)qrCodeWriter.Options).Version = Version.getVersionForNumber(comboBox_version.SelectedIndex);
            refreshQrCode();*/
        }
    }
}

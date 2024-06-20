using FireSharp;
using FireSharp.Config;
using FireSharp.Interfaces;
using FireSharp.Response;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace kabin_c_sharp
{
    public partial class Form1 : Form
    {
        IFirebaseClient istemci;

        IFirebaseConfig konfigurasyon = new FirebaseConfig
        {
            AuthSecret = "6KnsAxrGhLndlCHMCTVDN7cVUa0YHoSu4xqY6AU0",
            BasePath = "https://kabin-veriler-default-rtdb.firebaseio.com/"
        };

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            istemci.Set("011110007_ayar", textBox1.Text);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            istemci = new FirebaseClient(konfigurasyon);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            FirebaseResponse cevap = istemci.Get("011110007");
            label2.Text = cevap.Body;
        }
    }
}

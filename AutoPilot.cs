using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Collections.ObjectModel;
using System.Globalization;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Threading;

namespace AutoPilot
{

    public partial class AutoPilot : Form
    {
        string GsOut = string.Empty;
        string sIn_Cmds = string.Empty;

        string s_FileName_FromFS = "C:\\Temp\\FS\\FromFS.txt";
        string s_FileName_ToFS = "C:\\Temp\\FS\\ToFS.txt";

        public const int CONTROLS_COUNT = 14;
        double[] Gd_Controls_Value = new double[CONTROLS_COUNT];

        public const int ATTITUDE_PITCH = 0;
        public const int ATTITUDE_BANK = 1;
        public const int AIRSPEED = 2;
        public const int HEADING = 3;
        public const int ALTITUDE = 4;
        public const int ALTITUDE_GRN = 5;
        public const int VERTICAL_SPEED = 6;
        public const int YAW_G_FORCE = 7;
        public const int YOKE_X = 8;
        public const int YOKE_Y = 9;
        public const int RUDDER_POS = 10;
        public const int THROTTLE_POS = 11;
        public const int ELEVATOR_TRIM_POS = 12;

        bool bAuto_Pilot = true;

        bool b_0_Trim = false;



        public AutoPilot()
        {
            InitializeComponent();
        }
       
        private void timer1_Tick(object sender, EventArgs e)
        {
            string[] words;
            string sOut = ".";

            // Get commands
            int index = 0;
            if (File.Exists(s_FileName_FromFS)) 
            {
                try
                {
                    //Pass the file path and file name to the StreamReader constructor
                    StreamReader sr = new StreamReader(s_FileName_FromFS);
                    //Read the first line of text
                    sIn_Cmds = sr.ReadLine();
                    sr.Close();
                    File.Delete(s_FileName_FromFS);

                    sOut = string.Empty;
                    words = sIn_Cmds.Split(' ');
                    foreach (var word in words)
                    {
                        Gd_Controls_Value[index] = double.Parse(word);
                        index++;
                        if (index == 12)
                        {
                            bAuto_Pilot = true;
                        }
                        sOut += word + " ";
                    }

                    

                }
                catch
                {
                    System.Console.WriteLine("No File!");
                }

                label_AirSpeed.Text = Gd_Controls_Value[AIRSPEED].ToString("Airspeed: ###");
                //Refresh();
            }

            Auto_Pilot();
        }

        private void btn_Heading_Click(object sender, EventArgs e)
        {
            textBox_Heading.Text = Gd_Controls_Value[HEADING].ToString("###.#");
        }

        double d_Cmd_Altitude_Offset = 0.0;
        private void Auto_Pilot()
        {
            double d_Yoke_X, d_Yoke_Y, d_Rudder;

            double d_Yoke_Max_Y = 5.0;

            string sOut = string.Empty;
            string sIn = string.Empty;
            
            if (!bAuto_Pilot) return;
            
            GsOut = string.Empty;


            textBox_Trim_1.Text = Gd_Controls_Value[ELEVATOR_TRIM_POS].ToString("#.######");

            if (b_0_Trim)
            {
                b_0_Trim = false;
                GsOut += ELEVATOR_TRIM_POS.ToString() + " " + "0.0 ";
            }

            // Altitude Hold
            if (checkBox_Altitude.Checked)
            {
                d_Cmd_Altitude_Offset = PID_Altitude();

                if (d_Cmd_Altitude_Offset > d_Yoke_Max_Y) d_Cmd_Altitude_Offset = d_Yoke_Max_Y;
                if (d_Cmd_Altitude_Offset < -d_Yoke_Max_Y) d_Cmd_Altitude_Offset = -d_Yoke_Max_Y;

                textBox_Altitude_1.Text = d_Cmd_Altitude_Offset.ToString("###.###");
                label_Altitude.Text = Gd_Controls_Value[ALTITUDE].ToString("#####");

                d_Cmd_Altitude_Offset = 10;

                GsOut += YOKE_Y.ToString() + " " + d_Cmd_Altitude_Offset.ToString() + " ";

            }



            if (checkBox_Pitch.Checked && !checkBox_Altitude.Checked)
            {
                d_Yoke_Y = -((Gd_Controls_Value[VERTICAL_SPEED] + d_Cmd_Altitude_Offset) / 100);
                GsOut += VERTICAL_SPEED.ToString() + " " + d_Yoke_Y.ToString() + " ";
                textBox_Pitch_1.Text = Gd_Controls_Value[VERTICAL_SPEED].ToString("###.####");
                textBox_Pitch_2.Text = d_Yoke_Y.ToString("###.####");


                
                //textBox_Pitch_2.Text = d_Yoke_Y.ToString("###.####");
            }

            //---------------------------------------------------------------------------

            if (checkBox_Wings.Checked)
            {
                d_Yoke_X = Gd_Controls_Value[ATTITUDE_BANK];
                
                if (d_Yoke_X > 10.0) d_Yoke_X = 10.0;
                if (d_Yoke_X < -10.0) d_Yoke_X = -10.0;

                GsOut += YOKE_X.ToString() + " " + d_Yoke_X.ToString() + " ";
                textBox_Wings_1.Text = Gd_Controls_Value[ATTITUDE_BANK].ToString("###.###");
                textBox_Wings_2.Text = d_Yoke_X.ToString("###.####");
            }

            if (checkBox_Yaw.Checked)
            {
                d_Rudder = -(Gd_Controls_Value[YAW_G_FORCE] * 50.0);

                if (d_Rudder > 10.0) d_Rudder = 10.0;
                if (d_Rudder < -10.0) d_Rudder = -10.0;

                GsOut += RUDDER_POS.ToString() + " " + d_Rudder.ToString() + " ";
                textBox_Yaw_1.Text = Gd_Controls_Value[YAW_G_FORCE].ToString("###.####");
                textBox_Yaw_1.Text = d_Rudder.ToString("###.####");
            }

            if (GsOut != string.Empty)
            {
                try
                {
                    using (System.IO.StreamWriter file = new System.IO.StreamWriter(s_FileName_ToFS)) // Open a text file to update the content
                        file.Write(GsOut); // Write the content
                }
                catch
                {

                }
            }
                
        }

        private void AutoPilot_Load(object sender, EventArgs e)
        {

        }

        private void button_Altitude_Click(object sender, EventArgs e)
        {
            textBox_Altitude_2.Text = Gd_Controls_Value[ALTITUDE].ToString("#####");
        }

        private void button_0_Trim_Click(object sender, EventArgs e)
        {
            b_0_Trim = true;
        }


        // PID for Altitude Control
        private double PID_Altitude()
        {
            double d_Proportional;
            double d_Intergral = 0;
            double d_derivative = 0;

            double d_Altitude_Set, d_Altitude_Diff;
            double d_Temp;
            try
            {
                d_Altitude_Set = Convert.ToDouble(textBox_Altitude_2.Text);
            }
            catch
            {
                return 0;
            }

            d_Altitude_Diff = d_Altitude_Set - Gd_Controls_Value[ALTITUDE];

            textBox_Alt_Diff.Text = d_Altitude_Diff.ToString("####.#");

            d_Proportional = Alt_Proportional(d_Altitude_Diff);
            //d_Intergral = Alt_Intergral(d_Altitude_Diff);
            //d_derivative = Alt_Derivative(Gd_Controls_Value[ALTITUDE], d_Altitude_Diff);

            d_Temp = d_Proportional + d_Intergral + d_derivative;
            textBox_Alt_Sum.Text = d_Temp.ToString("##.####");
            return d_Temp;
        }

        //---------------------- Alt_Proportional
        private double Alt_Proportional(double d_Altitude_Diff)
        {
            const double d_MAX_DEFLECTION = 10.0;
            double d_PID_K_P;
            double d_Temp;


            try
            {
                d_PID_K_P = Convert.ToDouble(textBox_PID_K_P.Text);
            }
            catch
            {
                return 0;
            }
            d_Temp = d_Altitude_Diff * d_PID_K_P;

            if (d_Temp > d_MAX_DEFLECTION) d_Temp = d_MAX_DEFLECTION;
            if (d_Temp < -d_MAX_DEFLECTION) d_Temp = -d_MAX_DEFLECTION;

            textBox_Cal_P.Text = d_Temp.ToString("##.###");
            return d_Temp;
        }


        //---------------------- Alt_Intergral
        const int INTEGRAL_RANGE = 400;
        double[] Gd_Integral = new double[INTEGRAL_RANGE];
        static bool b_Alt_OneTime = true;
        static int i_Alt_Intergral_Index = 0;
        private double Alt_Intergral(double d_Altitude_Diff)
        {
            // Tunning
            double d_PID_K_I = 0.1;
            double d_Sum = 0;
            double d_Value;
            int c;

            try
            {
                d_PID_K_I = Convert.ToDouble(textBox_PID_K_I.Text);
            }
            catch
            {
                return 0;
            }

            if (b_Alt_OneTime)  // Fill in the first value
            {
                b_Alt_OneTime = false;
                for (c = 0; c < INTEGRAL_RANGE; c++) Gd_Integral[c] = d_Altitude_Diff;
            }

            Gd_Integral[i_Alt_Intergral_Index++] = d_Altitude_Diff;
            if (i_Alt_Intergral_Index == INTEGRAL_RANGE) i_Alt_Intergral_Index = 0; // Range check
            for (c = 0; c < INTEGRAL_RANGE; c++) d_Sum += Gd_Integral[c];

            d_Value = d_Sum * d_PID_K_I;
            textBox_Cal_I.Text = d_Value.ToString("##.####");

            return (d_Value);
        }


        //---------------------- Alt_Derivative
        static double d_Derivative_Val_Old = 9999.99;
        private double Alt_Derivative(double d_Altitude_Current, double d_Altitude_Diff)
        {
            double d_PID_K_D = 0.5;
            double d_Value;

            try
            {
                d_PID_K_D = Convert.ToDouble(textBox_PID_K_D.Text);
            }
            catch
            {
                return 0;
            }

            if (d_Derivative_Val_Old == 9999.99) d_Derivative_Val_Old = d_Altitude_Current;    // First time only

            d_Value = d_PID_K_D * (d_Altitude_Current - d_Derivative_Val_Old);
            d_Derivative_Val_Old = d_Altitude_Current;

            textBox_Cal_D.Text = d_Value.ToString("##.####"); 
            return d_Value;
        }

    }

}



namespace AutoPilot
{
    partial class AutoPilot
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AutoPilot));
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox_Heading = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.btn_Heading = new System.Windows.Forms.Button();
            this.label_Test1 = new System.Windows.Forms.Label();
            this.label_Test2 = new System.Windows.Forms.Label();
            this.checkBox_Pitch = new System.Windows.Forms.CheckBox();
            this.checkBox_Yaw = new System.Windows.Forms.CheckBox();
            this.textBox_Pitch_1 = new System.Windows.Forms.TextBox();
            this.checkBox_Bank = new System.Windows.Forms.CheckBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.checkBox_Altitude = new System.Windows.Forms.CheckBox();
            this.checkBox_Wings = new System.Windows.Forms.CheckBox();
            this.textBox_Pitch_2 = new System.Windows.Forms.TextBox();
            this.checkBox4 = new System.Windows.Forms.CheckBox();
            this.checkBox5 = new System.Windows.Forms.CheckBox();
            this.textBox_Wings_2 = new System.Windows.Forms.TextBox();
            this.textBox_Wings_1 = new System.Windows.Forms.TextBox();
            this.textBox_Yaw_2 = new System.Windows.Forms.TextBox();
            this.textBox_Yaw_1 = new System.Windows.Forms.TextBox();
            this.textBox_Altitude_1 = new System.Windows.Forms.TextBox();
            this.checkBox_Heading = new System.Windows.Forms.CheckBox();
            this.checkBox6 = new System.Windows.Forms.CheckBox();
            this.button_Altitude = new System.Windows.Forms.Button();
            this.textBox_Altitude_2 = new System.Windows.Forms.TextBox();
            this.label_Altitude = new System.Windows.Forms.Label();
            this.textBox_Trim_2 = new System.Windows.Forms.TextBox();
            this.textBox_Trim_1 = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.button_0_Trim = new System.Windows.Forms.Button();
            this.textBox_PID_K_I = new System.Windows.Forms.TextBox();
            this.textBox_PID_K_P = new System.Windows.Forms.TextBox();
            this.textBox_PID_K_D = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.textBox_Alt_Sum = new System.Windows.Forms.TextBox();
            this.textBox_Cal_I = new System.Windows.Forms.TextBox();
            this.textBox_Cal_P = new System.Windows.Forms.TextBox();
            this.textBox_Cal_D = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.textBox_Alt_Diff = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label_AirSpeed = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 200;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(184, 12);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(413, 26);
            this.textBox1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.SystemColors.Info;
            this.label1.Location = new System.Drawing.Point(184, 86);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(420, 175);
            this.label1.TabIndex = 1;
            this.label1.Text = "label1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.SystemColors.Info;
            this.label2.Location = new System.Drawing.Point(184, 313);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(51, 20);
            this.label2.TabIndex = 2;
            this.label2.Text = "label2";
            // 
            // textBox_Heading
            // 
            this.textBox_Heading.Location = new System.Drawing.Point(231, 619);
            this.textBox_Heading.Name = "textBox_Heading";
            this.textBox_Heading.Size = new System.Drawing.Size(82, 26);
            this.textBox_Heading.TabIndex = 3;
            this.textBox_Heading.Text = "-.-";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(725, 205);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(69, 20);
            this.label3.TabIndex = 4;
            this.label3.Text = "Heading";
            // 
            // btn_Heading
            // 
            this.btn_Heading.Location = new System.Drawing.Point(325, 616);
            this.btn_Heading.Name = "btn_Heading";
            this.btn_Heading.Size = new System.Drawing.Size(103, 34);
            this.btn_Heading.TabIndex = 5;
            this.btn_Heading.Text = "Heading";
            this.btn_Heading.UseVisualStyleBackColor = true;
            this.btn_Heading.Click += new System.EventHandler(this.btn_Heading_Click);
            // 
            // label_Test1
            // 
            this.label_Test1.AutoSize = true;
            this.label_Test1.Location = new System.Drawing.Point(767, 313);
            this.label_Test1.Name = "label_Test1";
            this.label_Test1.Size = new System.Drawing.Size(69, 20);
            this.label_Test1.TabIndex = 6;
            this.label_Test1.Text = "Heading";
            // 
            // label_Test2
            // 
            this.label_Test2.AutoSize = true;
            this.label_Test2.Location = new System.Drawing.Point(734, 251);
            this.label_Test2.Name = "label_Test2";
            this.label_Test2.Size = new System.Drawing.Size(69, 20);
            this.label_Test2.TabIndex = 7;
            this.label_Test2.Text = "Heading";
            // 
            // checkBox_Pitch
            // 
            this.checkBox_Pitch.AutoSize = true;
            this.checkBox_Pitch.Checked = true;
            this.checkBox_Pitch.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_Pitch.Location = new System.Drawing.Point(128, 522);
            this.checkBox_Pitch.Name = "checkBox_Pitch";
            this.checkBox_Pitch.Size = new System.Drawing.Size(70, 24);
            this.checkBox_Pitch.TabIndex = 8;
            this.checkBox_Pitch.Text = "Pitch";
            this.checkBox_Pitch.UseVisualStyleBackColor = true;
            // 
            // checkBox_Yaw
            // 
            this.checkBox_Yaw.AutoSize = true;
            this.checkBox_Yaw.Checked = true;
            this.checkBox_Yaw.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_Yaw.Location = new System.Drawing.Point(120, 759);
            this.checkBox_Yaw.Name = "checkBox_Yaw";
            this.checkBox_Yaw.Size = new System.Drawing.Size(66, 24);
            this.checkBox_Yaw.TabIndex = 9;
            this.checkBox_Yaw.Text = "Yaw";
            this.checkBox_Yaw.UseVisualStyleBackColor = true;
            // 
            // textBox_Pitch_1
            // 
            this.textBox_Pitch_1.Location = new System.Drawing.Point(237, 522);
            this.textBox_Pitch_1.Name = "textBox_Pitch_1";
            this.textBox_Pitch_1.Size = new System.Drawing.Size(82, 26);
            this.textBox_Pitch_1.TabIndex = 10;
            this.textBox_Pitch_1.Text = "-.-";
            // 
            // checkBox_Bank
            // 
            this.checkBox_Bank.AutoSize = true;
            this.checkBox_Bank.Checked = true;
            this.checkBox_Bank.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_Bank.Location = new System.Drawing.Point(120, 671);
            this.checkBox_Bank.Name = "checkBox_Bank";
            this.checkBox_Bank.Size = new System.Drawing.Size(72, 24);
            this.checkBox_Bank.TabIndex = 11;
            this.checkBox_Bank.Text = "Bank";
            this.checkBox_Bank.UseVisualStyleBackColor = true;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Checked = true;
            this.checkBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox1.Location = new System.Drawing.Point(128, 430);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(70, 24);
            this.checkBox1.TabIndex = 12;
            this.checkBox1.Text = "Pitch";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // checkBox_Altitude
            // 
            this.checkBox_Altitude.AutoSize = true;
            this.checkBox_Altitude.Checked = true;
            this.checkBox_Altitude.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_Altitude.Location = new System.Drawing.Point(128, 430);
            this.checkBox_Altitude.Name = "checkBox_Altitude";
            this.checkBox_Altitude.Size = new System.Drawing.Size(91, 24);
            this.checkBox_Altitude.TabIndex = 17;
            this.checkBox_Altitude.Text = "Attitude";
            this.checkBox_Altitude.UseVisualStyleBackColor = true;
            // 
            // checkBox_Wings
            // 
            this.checkBox_Wings.AutoSize = true;
            this.checkBox_Wings.Checked = true;
            this.checkBox_Wings.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_Wings.Location = new System.Drawing.Point(120, 671);
            this.checkBox_Wings.Name = "checkBox_Wings";
            this.checkBox_Wings.Size = new System.Drawing.Size(79, 24);
            this.checkBox_Wings.TabIndex = 16;
            this.checkBox_Wings.Text = "Wings";
            this.checkBox_Wings.UseVisualStyleBackColor = true;
            // 
            // textBox_Pitch_2
            // 
            this.textBox_Pitch_2.Location = new System.Drawing.Point(346, 522);
            this.textBox_Pitch_2.Name = "textBox_Pitch_2";
            this.textBox_Pitch_2.Size = new System.Drawing.Size(88, 26);
            this.textBox_Pitch_2.TabIndex = 15;
            this.textBox_Pitch_2.Text = "-.-";
            // 
            // checkBox4
            // 
            this.checkBox4.AutoSize = true;
            this.checkBox4.Checked = true;
            this.checkBox4.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox4.Location = new System.Drawing.Point(120, 759);
            this.checkBox4.Name = "checkBox4";
            this.checkBox4.Size = new System.Drawing.Size(66, 24);
            this.checkBox4.TabIndex = 14;
            this.checkBox4.Text = "Yaw";
            this.checkBox4.UseVisualStyleBackColor = true;
            // 
            // checkBox5
            // 
            this.checkBox5.AutoSize = true;
            this.checkBox5.Checked = true;
            this.checkBox5.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox5.Location = new System.Drawing.Point(128, 522);
            this.checkBox5.Name = "checkBox5";
            this.checkBox5.Size = new System.Drawing.Size(70, 24);
            this.checkBox5.TabIndex = 13;
            this.checkBox5.Text = "Pitch";
            this.checkBox5.UseVisualStyleBackColor = false;
            // 
            // textBox_Wings_2
            // 
            this.textBox_Wings_2.Location = new System.Drawing.Point(340, 671);
            this.textBox_Wings_2.Name = "textBox_Wings_2";
            this.textBox_Wings_2.Size = new System.Drawing.Size(88, 26);
            this.textBox_Wings_2.TabIndex = 19;
            this.textBox_Wings_2.Text = "-.-";
            // 
            // textBox_Wings_1
            // 
            this.textBox_Wings_1.Location = new System.Drawing.Point(231, 671);
            this.textBox_Wings_1.Name = "textBox_Wings_1";
            this.textBox_Wings_1.Size = new System.Drawing.Size(82, 26);
            this.textBox_Wings_1.TabIndex = 18;
            this.textBox_Wings_1.Text = "-.-";
            // 
            // textBox_Yaw_2
            // 
            this.textBox_Yaw_2.Location = new System.Drawing.Point(340, 757);
            this.textBox_Yaw_2.Name = "textBox_Yaw_2";
            this.textBox_Yaw_2.Size = new System.Drawing.Size(88, 26);
            this.textBox_Yaw_2.TabIndex = 21;
            this.textBox_Yaw_2.Text = "-.-";
            // 
            // textBox_Yaw_1
            // 
            this.textBox_Yaw_1.Location = new System.Drawing.Point(231, 757);
            this.textBox_Yaw_1.Name = "textBox_Yaw_1";
            this.textBox_Yaw_1.Size = new System.Drawing.Size(82, 26);
            this.textBox_Yaw_1.TabIndex = 20;
            this.textBox_Yaw_1.Text = "-.-";
            // 
            // textBox_Altitude_1
            // 
            this.textBox_Altitude_1.Location = new System.Drawing.Point(237, 430);
            this.textBox_Altitude_1.Name = "textBox_Altitude_1";
            this.textBox_Altitude_1.Size = new System.Drawing.Size(82, 26);
            this.textBox_Altitude_1.TabIndex = 22;
            this.textBox_Altitude_1.Text = "-.-";
            // 
            // checkBox_Heading
            // 
            this.checkBox_Heading.AutoSize = true;
            this.checkBox_Heading.Location = new System.Drawing.Point(120, 622);
            this.checkBox_Heading.Name = "checkBox_Heading";
            this.checkBox_Heading.Size = new System.Drawing.Size(95, 24);
            this.checkBox_Heading.TabIndex = 25;
            this.checkBox_Heading.Text = "Heading";
            this.checkBox_Heading.UseVisualStyleBackColor = true;
            // 
            // checkBox6
            // 
            this.checkBox6.AutoSize = true;
            this.checkBox6.Checked = true;
            this.checkBox6.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox6.Location = new System.Drawing.Point(120, 622);
            this.checkBox6.Name = "checkBox6";
            this.checkBox6.Size = new System.Drawing.Size(72, 24);
            this.checkBox6.TabIndex = 24;
            this.checkBox6.Text = "Bank";
            this.checkBox6.UseVisualStyleBackColor = true;
            // 
            // button_Altitude
            // 
            this.button_Altitude.Location = new System.Drawing.Point(447, 425);
            this.button_Altitude.Name = "button_Altitude";
            this.button_Altitude.Size = new System.Drawing.Size(103, 33);
            this.button_Altitude.TabIndex = 26;
            this.button_Altitude.Text = "Altitude";
            this.button_Altitude.UseVisualStyleBackColor = true;
            this.button_Altitude.Click += new System.EventHandler(this.button_Altitude_Click);
            // 
            // textBox_Altitude_2
            // 
            this.textBox_Altitude_2.Location = new System.Drawing.Point(346, 428);
            this.textBox_Altitude_2.Name = "textBox_Altitude_2";
            this.textBox_Altitude_2.Size = new System.Drawing.Size(82, 26);
            this.textBox_Altitude_2.TabIndex = 27;
            this.textBox_Altitude_2.Text = "0";
            // 
            // label_Altitude
            // 
            this.label_Altitude.Location = new System.Drawing.Point(346, 399);
            this.label_Altitude.Name = "label_Altitude";
            this.label_Altitude.Size = new System.Drawing.Size(82, 26);
            this.label_Altitude.TabIndex = 28;
            this.label_Altitude.Text = "Heading";
            this.label_Altitude.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // textBox_Trim_2
            // 
            this.textBox_Trim_2.Location = new System.Drawing.Point(346, 478);
            this.textBox_Trim_2.Name = "textBox_Trim_2";
            this.textBox_Trim_2.Size = new System.Drawing.Size(88, 26);
            this.textBox_Trim_2.TabIndex = 30;
            this.textBox_Trim_2.Text = "-.-";
            // 
            // textBox_Trim_1
            // 
            this.textBox_Trim_1.Location = new System.Drawing.Point(237, 478);
            this.textBox_Trim_1.Name = "textBox_Trim_1";
            this.textBox_Trim_1.Size = new System.Drawing.Size(82, 26);
            this.textBox_Trim_1.TabIndex = 29;
            this.textBox_Trim_1.Text = "-.-";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(180, 481);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(39, 20);
            this.label4.TabIndex = 31;
            this.label4.Text = "Trim";
            // 
            // button_0_Trim
            // 
            this.button_0_Trim.Location = new System.Drawing.Point(447, 478);
            this.button_0_Trim.Name = "button_0_Trim";
            this.button_0_Trim.Size = new System.Drawing.Size(103, 33);
            this.button_0_Trim.TabIndex = 32;
            this.button_0_Trim.Text = "0-Trim";
            this.button_0_Trim.UseVisualStyleBackColor = true;
            this.button_0_Trim.Click += new System.EventHandler(this.button_0_Trim_Click);
            // 
            // textBox_PID_K_I
            // 
            this.textBox_PID_K_I.Location = new System.Drawing.Point(771, 476);
            this.textBox_PID_K_I.Name = "textBox_PID_K_I";
            this.textBox_PID_K_I.Size = new System.Drawing.Size(82, 26);
            this.textBox_PID_K_I.TabIndex = 35;
            this.textBox_PID_K_I.Text = ".00001";
            // 
            // textBox_PID_K_P
            // 
            this.textBox_PID_K_P.Location = new System.Drawing.Point(771, 428);
            this.textBox_PID_K_P.Name = "textBox_PID_K_P";
            this.textBox_PID_K_P.Size = new System.Drawing.Size(82, 26);
            this.textBox_PID_K_P.TabIndex = 34;
            this.textBox_PID_K_P.Text = "0.01";
            // 
            // textBox_PID_K_D
            // 
            this.textBox_PID_K_D.Location = new System.Drawing.Point(771, 520);
            this.textBox_PID_K_D.Name = "textBox_PID_K_D";
            this.textBox_PID_K_D.Size = new System.Drawing.Size(82, 26);
            this.textBox_PID_K_D.TabIndex = 33;
            this.textBox_PID_K_D.Text = "0.01";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(717, 433);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(19, 20);
            this.label5.TabIndex = 37;
            this.label5.Text = "P";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Strikeout, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(717, 478);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(14, 20);
            this.label6.TabIndex = 38;
            this.label6.Text = "I";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Strikeout, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(717, 523);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(21, 20);
            this.label7.TabIndex = 39;
            this.label7.Text = "D";
            // 
            // textBox_Alt_Sum
            // 
            this.textBox_Alt_Sum.Location = new System.Drawing.Point(873, 567);
            this.textBox_Alt_Sum.Name = "textBox_Alt_Sum";
            this.textBox_Alt_Sum.Size = new System.Drawing.Size(82, 26);
            this.textBox_Alt_Sum.TabIndex = 43;
            this.textBox_Alt_Sum.Text = "-.-";
            // 
            // textBox_Cal_I
            // 
            this.textBox_Cal_I.Location = new System.Drawing.Point(873, 476);
            this.textBox_Cal_I.Name = "textBox_Cal_I";
            this.textBox_Cal_I.Size = new System.Drawing.Size(82, 26);
            this.textBox_Cal_I.TabIndex = 42;
            this.textBox_Cal_I.Text = "-.-";
            // 
            // textBox_Cal_P
            // 
            this.textBox_Cal_P.Location = new System.Drawing.Point(873, 428);
            this.textBox_Cal_P.Name = "textBox_Cal_P";
            this.textBox_Cal_P.Size = new System.Drawing.Size(82, 26);
            this.textBox_Cal_P.TabIndex = 41;
            this.textBox_Cal_P.Text = "-.-";
            // 
            // textBox_Cal_D
            // 
            this.textBox_Cal_D.Location = new System.Drawing.Point(873, 520);
            this.textBox_Cal_D.Name = "textBox_Cal_D";
            this.textBox_Cal_D.Size = new System.Drawing.Size(82, 26);
            this.textBox_Cal_D.TabIndex = 40;
            this.textBox_Cal_D.Text = "-.-";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(795, 399);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(24, 20);
            this.label8.TabIndex = 44;
            this.label8.Text = "K-";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(869, 399);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(84, 20);
            this.label9.TabIndex = 45;
            this.label9.Text = "Calculated";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(636, 399);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(38, 20);
            this.label10.TabIndex = 47;
            this.label10.Text = "Diff.";
            // 
            // textBox_Alt_Diff
            // 
            this.textBox_Alt_Diff.Location = new System.Drawing.Point(614, 428);
            this.textBox_Alt_Diff.Name = "textBox_Alt_Diff";
            this.textBox_Alt_Diff.Size = new System.Drawing.Size(82, 26);
            this.textBox_Alt_Diff.TabIndex = 46;
            this.textBox_Alt_Diff.Text = "-.-";
            // 
            // label11
            // 
            this.label11.Location = new System.Drawing.Point(0, 0);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(100, 23);
            this.label11.TabIndex = 0;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(783, 573);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(61, 20);
            this.label12.TabIndex = 48;
            this.label12.Text = "Sum-->";
            // 
            // label13
            // 
            this.label13.Location = new System.Drawing.Point(0, 0);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(100, 23);
            this.label13.TabIndex = 0;
            // 
            // label_AirSpeed
            // 
            this.label_AirSpeed.Location = new System.Drawing.Point(342, 324);
            this.label_AirSpeed.Name = "label_AirSpeed";
            this.label_AirSpeed.Size = new System.Drawing.Size(116, 26);
            this.label_AirSpeed.TabIndex = 49;
            this.label_AirSpeed.Text = "AirSpeed: ";
            this.label_AirSpeed.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // AutoPilot
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1024, 793);
            this.Controls.Add(this.label_AirSpeed);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.textBox_Alt_Diff);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.textBox_Alt_Sum);
            this.Controls.Add(this.textBox_Cal_I);
            this.Controls.Add(this.textBox_Cal_P);
            this.Controls.Add(this.textBox_Cal_D);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textBox_PID_K_I);
            this.Controls.Add(this.textBox_PID_K_P);
            this.Controls.Add(this.textBox_PID_K_D);
            this.Controls.Add(this.button_0_Trim);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textBox_Trim_2);
            this.Controls.Add(this.textBox_Trim_1);
            this.Controls.Add(this.label_Altitude);
            this.Controls.Add(this.textBox_Altitude_2);
            this.Controls.Add(this.button_Altitude);
            this.Controls.Add(this.checkBox_Heading);
            this.Controls.Add(this.checkBox6);
            this.Controls.Add(this.textBox_Altitude_1);
            this.Controls.Add(this.textBox_Yaw_2);
            this.Controls.Add(this.textBox_Yaw_1);
            this.Controls.Add(this.textBox_Wings_2);
            this.Controls.Add(this.textBox_Wings_1);
            this.Controls.Add(this.checkBox_Altitude);
            this.Controls.Add(this.checkBox_Wings);
            this.Controls.Add(this.textBox_Pitch_2);
            this.Controls.Add(this.checkBox4);
            this.Controls.Add(this.checkBox5);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.checkBox_Bank);
            this.Controls.Add(this.textBox_Pitch_1);
            this.Controls.Add(this.checkBox_Yaw);
            this.Controls.Add(this.checkBox_Pitch);
            this.Controls.Add(this.label_Test2);
            this.Controls.Add(this.label_Test1);
            this.Controls.Add(this.btn_Heading);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textBox_Heading);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "AutoPilot";
            this.Text = "AutoPilot";
            this.Load += new System.EventHandler(this.AutoPilot_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox_Heading;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btn_Heading;
        private System.Windows.Forms.Label label_Test1;
        private System.Windows.Forms.Label label_Test2;
        private System.Windows.Forms.CheckBox checkBox_Pitch;
        private System.Windows.Forms.CheckBox checkBox_Yaw;
        private System.Windows.Forms.TextBox textBox_Pitch_1;
        private System.Windows.Forms.CheckBox checkBox_Bank;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.CheckBox checkBox_Altitude;
        private System.Windows.Forms.CheckBox checkBox_Wings;
        private System.Windows.Forms.TextBox textBox_Pitch_2;
        private System.Windows.Forms.CheckBox checkBox4;
        private System.Windows.Forms.CheckBox checkBox5;
        private System.Windows.Forms.TextBox textBox_Wings_2;
        private System.Windows.Forms.TextBox textBox_Wings_1;
        private System.Windows.Forms.TextBox textBox_Yaw_2;
        private System.Windows.Forms.TextBox textBox_Yaw_1;
        private System.Windows.Forms.TextBox textBox_Altitude_1;
        private System.Windows.Forms.CheckBox checkBox_Heading;
        private System.Windows.Forms.CheckBox checkBox6;
        private System.Windows.Forms.Button button_Altitude;
        private System.Windows.Forms.TextBox textBox_Altitude_2;
        private System.Windows.Forms.Label label_Altitude;
        private System.Windows.Forms.TextBox textBox_Trim_2;
        private System.Windows.Forms.TextBox textBox_Trim_1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button button_0_Trim;
        private System.Windows.Forms.TextBox textBox_PID_K_I;
        private System.Windows.Forms.TextBox textBox_PID_K_P;
        private System.Windows.Forms.TextBox textBox_PID_K_D;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBox_Alt_Sum;
        private System.Windows.Forms.TextBox textBox_Cal_I;
        private System.Windows.Forms.TextBox textBox_Cal_P;
        private System.Windows.Forms.TextBox textBox_Cal_D;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBox_Alt_Diff;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label_AirSpeed;
    }
}


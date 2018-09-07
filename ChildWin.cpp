//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Printers.hpp>

#include <fstream.h>
#include "Mydata.h"
#include "Myplot20.h"
#include "ChildWin.h"
#include "ConfirmDeleteFrm.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------
__fastcall TDataFile::TDataFile(TComponent *Owner)
	: TForm(Owner),HeaderSize(4096)
{
    Dat = new DataArray();
//    Plot = new MyPlot();
    AverDat = new DataArray();
//    AverPlot = new MyPlot();
	Header = new char[HeaderSize];
    Saved = false;
    FocusRect = 0;
	AverageOn = false;
	Measuring = false;

    nPlots = 1;
    Plots = new MyPlot*[4];              // NB an array of plots
    for (int i =0; i<4; i++)
        Plots[i] = new MyPlot();
//    Plot = Plots[0];
//    AverPlot = Plots[1];
                                        // All Plots are initialized !
    nDatas = 1;
    Datas = new DataArray*[4];           // NB an array of data arrays
    Datas[0] = Dat;
    Datas[1] = AverDat;                  // Averdat remains not initialized
    Datas[2] = NULL;
    Datas[3] = NULL;

    // in case of multiple plot, gives a clue to positions
    // on the canvas. Default just the half

    PlotCenter.Set(0,0.5);                // Set center-X for 4 plots
    PlotCenter.Set(1,0.5);                // Set center-Y for 4 or 2 plots

}
//---------------------------------------------------------------------
void __fastcall TDataFile::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;            //caFree
}
//---------------------------------------------------------------------
void __fastcall TDataFile::FormPaint(TObject *Sender)
{
    PlotData();
}
//---------------------------------------------------------------------------

void __fastcall TDataFile::FormResize(TObject *Sender)
{
    Invalidate();
}
//---------------------------------------------------------------------------


void __fastcall TDataFile::PlotAxes()
{
    // Resize Plot depending on window size
    // and whether Averaging or not
    int Xc, Yc;
    Xc = PlotCenter.Get(0);
    Yc = PlotCenter.Get(1);

    if (nPlots == 2)
    {
        Plots[0]->SetLim(0,0, ClientWidth, Yc * ClientHeight);
        Plots[1]->SetLim(0,Yc*ClientHeight, ClientWidth, ClientHeight);
    }
       else
       {
        if (nPlots == 4)
        {
            Plots[0]->SetLim(0,0, Xc*ClientWidth, Yc * ClientHeight);
            Plots[1]->SetLim(0,Yc*ClientHeight, Xc*ClientWidth, ClientHeight);
            Plots[2]->SetLim(Xc*ClientWidth , 0, ClientWidth, Yc * ClientHeight);
            Plots[3]->SetLim(Xc*ClientWidth ,Yc*ClientHeight, ClientWidth, ClientHeight);
        }
          else
          {
            Plots[0]->SetLim(ClientWidth, ClientHeight);
          }
       }

    for (int i = 0; i< nPlots; i++)
        Plots[i]->PlotTheAxes(Canvas);

}

//-------------------------------------------------------------------------------------

void __fastcall TDataFile::PrintData()
{
    if (Plots[0] == NULL) return;
    // Resize Plot depending on window size
    // and whether Averaging or not

    if (nDatas == 4)                   // both averaged and updown
                            // Plot: Only averaged Data
    {
        Plots[0]->SetLim(Printer()->PageWidth, Printer()->PageHeight/4);
        Plots[1]->SetLim(0, Printer()->PageHeight/4,
                    Printer()->PageWidth,Printer()->PageHeight/2);
    	Plots[0]->SetRanges(Datas[1]->Minima(), Datas[1]->Maxima());
    	Plots[1]->SetRanges(Datas[3]->Minima(), Datas[3]->Maxima());
    	Plots[0]->PlotTheAxes(Printer()->Canvas);
        Plots[1]->PlotTheAxes(Printer()->Canvas);
        if (Datas[1]->Getn() >0)
            Plots[0]->PlotArray(Printer()->Canvas, *Datas[1]);
        if (Datas[3]->Getn() >0)
	        Plots[1]->PlotArray(Printer()->Canvas, *Datas[3]);
    }
    else
        if ((nDatas == 2) && (UpDown))  // Print Both up down
        {
            Plots[0]->SetLim(Printer()->PageWidth, Printer()->PageHeight/4);
            Plots[1]->SetLim(0, Printer()->PageHeight/4,
                    Printer()->PageWidth,Printer()->PageHeight/2);
			Plots[0]->SetRanges(Datas[0]->Minima(), Datas[0]->Maxima());
    	    Plots[1]->SetRanges(Datas[1]->Minima(), Datas[1]->Maxima());
    	    Plots[0]->PlotTheAxes(Printer()->Canvas);
            Plots[1]->PlotTheAxes(Printer()->Canvas);
            if (Datas[0]->Getn() >0)
	               Plots[0]->PlotArray(Printer()->Canvas, *Datas[0]);
	        if (Datas[1]->Getn() >0)
	               Plots[1]->PlotArray(Printer()->Canvas, *Datas[1]);
        }
          else
			if (nDatas == 2)            // Print only average
            {
                Plots[0]->SetLim(Printer()->PageWidth, Printer()->PageHeight/2);
                Plots[0]->SetRanges(Datas[1]->Minima(), Datas[1]->Maxima());
    	        Plots[0]->PlotTheAxes(Printer()->Canvas);
                if (Datas[1]->Getn() >0)
       	            Plots[0]->PlotArray(Printer()->Canvas, *Datas[1]);
    	    }
              else
                {
                    Plots[0]->SetLim(Printer()->PageWidth, Printer()->PageHeight/2);
                    Plots[0]->SetRanges(Datas[0]->Minima(), Datas[0]->Maxima());
    	            Plots[0]->PlotTheAxes(Printer()->Canvas);
                    if (Datas[0]->Getn() >0)
       	                Plots[0]->PlotArray(Printer()->Canvas, *Datas[0]);
    	        }

}

//----------------------------------------------------------------------------

void __fastcall TDataFile::PlotData()
{
    if (Plots[0] == NULL) return;

    double Xc, Yc;
    Xc = PlotCenter.Get(0);
    Yc = PlotCenter.Get(1);

    // Resize Plot depending on window size
    // and whether Averaging or not

    if (nPlots == 2)
    {
        Plots[0]->SetLim(0,0, ClientWidth, Yc * ClientHeight);
        Plots[1]->SetLim(0,Yc*ClientHeight, ClientWidth, ClientHeight);
    }
       else
       {
        if (nPlots == 4)
        {
            Plots[0]->SetLim(0,0, Xc*ClientWidth, Yc * ClientHeight);
            Plots[1]->SetLim(0,Yc*ClientHeight, Xc*ClientWidth, ClientHeight);
            Plots[2]->SetLim(Xc*ClientWidth , 0, ClientWidth, Yc * ClientHeight);
            Plots[3]->SetLim(Xc*ClientWidth ,Yc*ClientHeight, ClientWidth, ClientHeight);
        }
          else
          {
            Plots[0]->SetLim(ClientWidth, ClientHeight);
          }
       }


    for (int i = 0; i< nPlots; i++)
    {
        Plots[i]->SetRanges(Datas[i]->Minima(), Datas[i]->Maxima());
        Plots[i]->PlotTheAxes(Canvas);
        if (Datas[i]->Getn() > 0) Plots[i]->PlotArray(Canvas, *Datas[i]);
    }
/*
    if (nPlots == 2)
    {
        Plot->SetLim(0,0, ClientWidth, PlotCenter.Get(1) * ClientHeight);
        AverPlot->SetLim(0,PlotCenter.Get(1)*ClientHeight, ClientWidth, ClientHeight);
    	Plot->SetRanges(Dat->Minima(), Dat->Maxima());
    	AverPlot->SetRanges(AverDat->Minima(), AverDat->Maxima());
    	Plot->PlotTheAxes(Canvas);
    	AverPlot->PlotTheAxes(Canvas);
        if (Dat->Getn() >0)
	    {
       	    Plot->PlotArray(Canvas, *Dat);
	    }
        if (AverDat->Getn() >0)
	    {
       	    AverPlot->PlotArray(Canvas, *AverDat);
	    }
    }
    else
      {
        Plot->SetLim(ClientWidth, ClientHeight);
    	Plot->SetRanges(Dat->Minima(), Dat->Maxima());
    	Plot->PlotTheAxes(Canvas);
        if (Dat->Getn() >0)
	    {
       	    Plot->PlotArray(Canvas, *Dat);
	    }
      }
*/
}

/////////////////////////////////////////////////
//
//  WriteData
//
//      Parameters
//        ANsiString     Filename   Name of outputfile
//                                  usually from SaveDialog
//      Return value
//        int   -1 if file was not opened (error)
//              number of datapoints written
//
int TDataFile::WriteData(AnsiString Filename)
{
    ofstream data(Filename.c_str());
    if (!data) return -1;   // if unsuccesful in opening the file return -1
//  Here we should write the header
    data << Header;
//  Here we should indicate the START of Data
    data << ">BEGIN" << endl;
//  Here we should write the data

    for (int i=0; i< Dat->Getn(); i++)
    {
        data << Dat->Get(i);
        data << endl;
    }

    data.close();        // close data file
    return Dat->Getn();  // return #data written
}

//--------------------------------------------------------------------

int TDataFile::WriteData(int mode, AnsiString Filename)
{
    ofstream data(Filename.c_str());
    if (!data) return -1;   // if unsuccesful in opening the file return -1
//  Here we should write the header
    data << Header;
//  Here we should indicate the START of Data
    data << ">BEGIN" << endl;
//  Here we should write the data
    int ndat = 0;

    if (mode < nDatas)
    {
        for (int i=0; i< Datas[mode]->Getn(); i++)
        {
            data << Datas[mode]->Get(i);
            data << endl;
        }
        ndat = Datas[mode]->Getn();
    }

    data.close();        // close data file
    return ndat;  // return #data written
}

//-------------------------------------------------------------------------

int TDataFile::ReadData(AnsiString Filename)
{

// Open the data file
    ifstream data(Filename.c_str());
    if (!data) return -1;
    if (data.eof()) return 0;

    int ndata = 0;
    DataArray *TempDat = NULL;
    const int maxline = 512;         // each real should be about 15 characters  long
    char* line = new char[maxline];
    DataPoint P;
   	double x;
	double y[MAXNY];    // NB MAXNY is now pretty big (200 ?)
	int nItems;

    //  Get the first line
    data.getline(line, maxline);
    if (data.fail())
    {
        delete[] line;
        return -1;
    }
    if ((strncmp(line, "Measurement made ",17) == 0) || (strncmp(line, "DataFile HansFormat",17) == 0))
    {
        //  This should be a good measurement file
        //  which includes a header. Let's read the header.

        strcat(line,"\n");  //put a return at the end
        SetHeader(line);

        int curpos = data.tellg();
        data.seekg(0,ios_base::end);
        int nbytes = data.tellg();

        data.seekg(curpos);

        int StopReadHeader = 0;
        while ((!data.eof()) && (StopReadHeader==0))
        {
            data.getline(line, maxline);   //contrary to get(line, maxline) the eol('\n') character is read
            if (data.fail())
            {
                delete[] line;
                data.close();
                return -1;
            }
            if (strncmp(line, ">BEGIN",6) == 0) StopReadHeader = 1;
                            //Serves as a more elegant break to break the loop
              else
              {
                strcat(line,"\n");
                AddHeader(line);
              }
        }
        if (data.eof()) return 0;

// Now read the data

// one of the problems is to know how many points there
// are. With MAXNY and MAXPNT so big, we get something
// like 8 * 65000 * 200 = 104 Mb  reserved space !!
// So we'll have to limit that.
//  I do it rather crudely here by creating an temporary
//  DataFile and reading that in. The copy to the
//  Childwin datafile ensures optimal use of memory

        if ((!data.fail()) && (!data.eof()))
        {
            data >> P;
	    	if ( (!data.eof()) && (!data.fail()))
		    {
	            if ((TempDat == NULL) || (TempDat->Getn() == 0))    // if first datapoint
                {
                    if (TempDat != NULL) delete TempDat;
                    if ((P.GetNy() > 10) && (P.GetNy() <=20))
                        TempDat = new DataArray(16396,P.GetNy());
                      else
                      {
                        if (P.GetNy() >20)
                             TempDat = new DataArray(8198,P.GetNy());
                          else
                            TempDat = new DataArray(MAXPNT,P.GetNy());
                      }
                }
			    TempDat->Add(P);
                ndata++;
    		}
	    }

        while ((!data.fail()) && (!data.eof()))
        {
            data >> P;
	    	if ( (!data.eof()) && (!data.fail()))
		    {
			    TempDat->Add(P);
                ndata++;
    		}
	    }

        if (TempDat != NULL)
        {
            *Datas[0] = *TempDat;
            delete TempDat;          // cleanup
            TempDat = NULL;
        }
        delete[] line;
        data.close();
        Saved = true;
        nPlots = 1;
        nDatas = 1;
        return ndata;
	}
    // If not, let's try if we have a good ASCII data file
	nItems = sscanf(line,
		"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
		&x, y, y+1, y+2, y+3, y+4, y+5, y+6, y+7, y+8, y+9, y+10, y+11, y+12,
			y+13,y+14, y+15,y+16, y+17,y+18, y+19);
	if ((strlen(line) > 2) && (nItems > 1) && (nItems <= MAXNY+1))
    {
        // reset the pointer
        data.seekg(0);
        if ((!data.fail()) && (!data.eof()))
        {
            data >> P;
	    	if ( (!data.eof()) && (!data.fail()))
		    {
			    if (TempDat == NULL)    // if first datapoint
                {
                    TempDat = new DataArray(MAXPNT,P.GetNy());
                }
			    TempDat->Add(P);
                ndata++;
    		}
	    }

        while ((!data.fail()) && (!data.eof()))
        {
            data >> P;
	    	if ( (!data.eof()) && (!data.fail()))
		    {
			    TempDat->Add(P);
                ndata++;
    		}
	    }

        if (TempDat != NULL)
        {
            *Datas[0] = *TempDat;
            delete TempDat;          // cleanup
            TempDat = NULL;
        }
	}

    delete[] line;
    data.close();
    Saved = true;
    nPlots = 1;
    nDatas = 1;
    return ndata;
}

//--------------------------------------------------------------------------

bool TDataFile::IsEmpty()
{
    for (int i=0; i<nDatas; i++)
		{if (Datas[i]->Getn() > 0) return false;}
	return true;
}

//---------------------------------------------------------------------------

void TDataFile::DataReset(int dim, int npoints)
{
    // NB Resets only Dat datarray;
    delete Dat;
    Dat = new DataArray(npoints, dim);
    Datas[0] = Dat;
    return;
}

void TDataFile::AllDataReset(int dim, int npoints)
{
    // NB Resets all datarrays;

    if ((AverageOn) && (UpDown))
    {
        nDatas = 4;
        nPlots = 4;
    }
      else
        if ((AverageOn) || (UpDown))
        {
            nDatas = 2;
            nPlots = 2;
        }
          else
            {
                nDatas = 1;
                nPlots = 1;
            }
    for (int i=0; i<4; i++)
        if (Datas[i] != NULL)
        {
            delete Datas[i];
            Datas[i] = NULL;
        }
    for (int i=0; i<nDatas; i++)
        Datas[i] = new DataArray(npoints, dim);
    Dat = Datas[0];
    AverDat = Datas[1];
    return;
}


// This version resets one of the arrays
// NB No default value for npoints !!
void TDataFile::DataReset(int dataset, int dim, int npoints)
{
    if (Datas[dataset] != NULL) delete Datas[dataset];
    Datas[dataset] = new DataArray(npoints, dim);
    return;
}
//----------------------------------------------------------------------------

void __fastcall TDataFile::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if (Measuring) {
		CanClose = false;
		return;
	}
	if ((IsEmpty()) || Saved) CanClose=true;
        else
        {
            DeleteConfirmDialog->ShowModal();
            if (DeleteConfirmDialog->ModalResult == mbOK)
                CanClose = true;
              else CanClose = false;
        }
}
//---------------------------------------------------------------------------


void __fastcall TDataFile::FormDestroy(TObject *Sender)
{
    for (int i=0; i<nDatas; i++) delete Datas[i];
    Dat = NULL;
    for (int i=0; i<nPlots; i++) delete Plots[i];
    Plot = NULL;
    AverDat = NULL;
    AverPlot = NULL;
    delete [] Plots;
    delete [] Datas;
//    delete[] Header;
//    Header = NULL;
}
//---------------------------------------------------------------------------

int TDataFile::SetHeader(const char* text)
{
    strcpy(Header, text);
    Header[HeaderSize-1] = 0;
    return strlen(Header);
}

//--------------------------------------------------------------------------
int TDataFile::AddHeader(const char* text)
{
    if ((strlen(Header)+strlen(text)) < (unsigned int)HeaderSize)
    {
        strcat(Header, text);
    }
    return strlen(Header);
}

//-------------------------------------------------------------------------

int TDataFile::GetHeader(char* H, int nchar)
{
    strncpy(H, Header, nchar);
    return (int)strlen(H);
}

//-------------------------------------------------------------------------

void __fastcall TDataFile::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{


	if (Button == mbLeft)
	{
		char HintMessage[40];

		Screen->Cursor = crCross;
		TPoint Pscreen(X,Y);
		Screen->Cursor = crCross;


		DataPoint Puser(Plots[GetCursorPlot(X,Y)]->GetUserCoordinates(Pscreen));

		sprintf(HintMessage,"( %g , %g )",Puser.Get(0), Puser.Get(1));

		Hint = HintMessage;
	}

	if (Button == mbRight) // if right MouseButton pressed
	{
		Screen->Cursor = crSizeNWSE;
		FocusRect = new TRect();
        FocusRect->Left = X;
        FocusRect->Right = X;
        FocusRect->Top = Y;
        FocusRect->Bottom = Y;
        Canvas->DrawFocusRect(*FocusRect);
    }
}

//
int TDataFile::GetCursorPlot(int X, int Y) const
{
	int Xc = PlotCenter.Get(0)*ClientWidth;
	int Yc = PlotCenter.Get(1)*ClientHeight;
    if (nPlots == 4)
    {
        if (X<=Xc)
            if (Y<=Yc) return 0; else return 1;
        else  // X>Xc
            if (Y<=Yc) return 2; else return 3;
    }
      else
      {
        if (nPlots == 2)
            if (Y<=Yc) return 0; else return 1;
        else return 0;
      }
}

//---------------------------------------------------------------------------

void __fastcall TDataFile::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	if (!FocusRect)
        {
                if (Shift.Contains(ssLeft))
                {
                        char HintMessage[40];
                        TPoint Pscreen(X,Y);
                        DataPoint Puser(Plots[GetCursorPlot(X,Y)]->GetUserCoordinates(Pscreen));
                        sprintf(HintMessage,"( %g , %g )",Puser.Get(0), Puser.Get(1));
                        Hint = HintMessage;
                }
                return;
        }
	Canvas->DrawFocusRect(*FocusRect);
	FocusRect->Right = X;
        FocusRect->Bottom = Y;
	Canvas->DrawFocusRect(*FocusRect);
}
//---------------------------------------------------------------------------

void __fastcall TDataFile::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Screen->Cursor = crDefault;

	int iPlot = GetCursorPlot(X,Y);
	if (!FocusRect)
        {
                return;
        }
	Canvas->DrawFocusRect(*FocusRect);
	if ((FocusRect->Right > FocusRect->Left+3) && (FocusRect->Bottom > FocusRect->Top + 3))
	{
		Plots[iPlot]->SetAutoRange(0);
		TPoint BottomLeft(FocusRect->Left, FocusRect->Bottom);
		TPoint TopRight(FocusRect->Right, FocusRect->Top);
		Plots[iPlot]->SetFixedRanges(Plots[iPlot]->GetUserCoordinates(BottomLeft),
                   		Plots[iPlot]->GetUserCoordinates(TopRight));
		Invalidate();
	}
	delete FocusRect;
	FocusRect = 0;
}
//---------------------------------------------------------------------------
void TDataFile::SetAverage(int Av)
{
    if (Av>0) AverageOn = true;
      else AverageOn = false;
}

void TDataFile::SetUpDown(int UpDo)
{
    if (UpDo>0) UpDown = true;
      else UpDown = false;
}

MyPlot* TDataFile::GetPlot(int i)
{
    return Plots[i];
}

DataArray* TDataFile::GetData(int i)
{
	if ((i>=0) && (i<4)) return Datas[i];
	return NULL;
}

int TDataFile::SetData(DataArray NewData)
{
    *Dat = NewData;
    Invalidate();
    return 0;
}

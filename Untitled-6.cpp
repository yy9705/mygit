#include <iostream>

using namespace std;

int main()
{
    int d[50],b[4],sum = 0;
    char dd[50];
    int i,j;
    cout<<"Enter a num";
    cin>>dd;
    for(i = 0;dd[i] > 0;i++)
    {
        if(dd[i] >= 48 && dd[i] <= 57)
        d[i] = dd[i] - 48;
        else if(dd[i] >= 65 && dd[i] <= 70)
        d[i] = dd[i] - 55;
        else if(dd[i] >= 97 && dd[i] <= 102)
        d[i] = dd[i] - 87;
        cout<<"NO."<<i<<" "<<dd[i]<<" "<<d[i]<<endl;
        sum = sum*16 + d[i];
    }
    cout<<"D"<<" "<<sum<<endl;
    cout<<"H"<<" "<<dd<<endl;
    cout<<"B"<<" ";
    for(i = 3;i > -1;i--)
    {
        for(j = 0;j < 4;j++)
        {
            b[j] = d[i]%2;
            d[i] = (d[i] - b[j])/2;
        }
        for(j = 3;j>-1;j--)
        {
            cout<<b[j];
        }
        cout<<" ";
    }
    cout<<endl;
    return 0;
}
using System.Windows;
using Logic;
using Data;
using Model;
using ViewModel;

namespace View
{
    public partial class App : System.Windows.Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            // 1. DANE
            DataAbstractApi dataApi = DataAbstractApi.CreateApi(800, 400);

            // 2. LOGIKA
            LogicAbstractApi logicApi = LogicAbstractApi.CreateApi(dataApi);
           

            // 3. MODEL
            MainModel model = new MainModel(logicApi);

            // 4. VIEWMODEL
            MainViewModel viewModel = new MainViewModel(model);

            // 5. VIEW
            MainWindow window = new MainWindow();
            window.DataContext = viewModel;
            window.Show();
        }
    }
}
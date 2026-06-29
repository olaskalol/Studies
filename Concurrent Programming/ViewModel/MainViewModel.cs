using Data;
using Model;
using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Threading;
using System.Windows.Input;
using System.Diagnostics;
using System.Threading.Tasks;

namespace ViewModel
{
    public class MainViewModel : INotifyPropertyChanged
    {
        private readonly MainModel model;
        private int numberOfBalls = 5;
        private readonly SynchronizationContext syncContext;
        private Stopwatch stopwatch = new Stopwatch();
        private string runtime = "00:00:00";
        public ObservableCollection<BallModel> Balls { get; } = new ObservableCollection<BallModel>();

        private string errorMessage = string.Empty;

        public string ErrorMessage
        {
            get => errorMessage;
            set
            {
                errorMessage = value;
                OnPropertyChanged(nameof(ErrorMessage));
            }
        }

        public string Runtime
        {
            get => runtime;
            set
            {
                runtime = value;
                OnPropertyChanged(nameof(Runtime));
            }
        }

        public int NumberOfBalls
        {
            get => numberOfBalls;
            set
            {
                if (value > 200)
                {
                    numberOfBalls = 200;
                    ErrorMessage = "Możesz wpisać maksymalnie 200 kul.";
                }
                else if (value < 0)
                {
                    numberOfBalls = 0;
                    ErrorMessage = "Liczba kul nie może być ujemna.";
                }
                else
                {
                    numberOfBalls = value;
                    ErrorMessage = string.Empty; 
                }
                OnPropertyChanged(nameof(NumberOfBalls));
            }
        }

        public ICommand StartCommand { get; }
        public ICommand StopCommand { get; }

        public MainViewModel(MainModel inModel)
        {
            model = inModel;

            syncContext = SynchronizationContext.Current ?? new SynchronizationContext();

            StartCommand = new Command(Start);
            StopCommand = new Command(Stop);

            model.PositionChanged += OnPositionChanged;
        }
        private void OnPositionChanged(object sender, EventArgs e)
        {
            if (stopwatch.IsRunning)
            {
                syncContext.Post(_ =>
                {
                    Runtime = stopwatch.Elapsed.ToString(@"hh\:mm\:ss");
                }, null);
            }
        }

        private void Start() 
        {
            stopwatch.Restart();

            model.Start(NumberOfBalls);

            Balls.Clear();
            foreach (var ball in model.GetBalls())
            {
                Balls.Add(new BallModel(ball));
            }
        }

        private void Stop()
        {
            model.Stop();
            stopwatch.Stop();
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
using Data;
using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Threading;
namespace Model
{
    public class BallModel : INotifyPropertyChanged
    {
        private readonly IBall logicBall;
        private readonly SynchronizationContext? _syncContext;
        public double X => logicBall.position.X;
        public double Y => logicBall.position.Y;
        public double Diameter => logicBall.radius*2;

        public event PropertyChangedEventHandler? PropertyChanged;

        public BallModel(IBall LogicBall)
        {
            logicBall = LogicBall;
            _syncContext = SynchronizationContext.Current;
            logicBall.BallChanged += OnBallChanged;
        }

        private void OnBallChanged(object? sender, EventArgs e)
        {
            if (_syncContext != null)
            {
                _syncContext.Post(_ => Update(), null);
            }
            else
            {
                Update();
            }
        }

        public void Update()
        {
            OnPropertyChanged(nameof(X));
            OnPropertyChanged(nameof(Y));
        }

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}


using System;
using System.Windows.Input;

namespace ViewModel
{
    public class Command : ICommand
    {
        private readonly Action execute;
        private readonly Func<bool> bCanExecute;

        public Command(Action inExecute, Func<bool> inCanExecute = null)
        {
            execute = inExecute ?? throw new ArgumentNullException(nameof(execute));
            bCanExecute = inCanExecute;
        }

        public event EventHandler CanExecuteChanged;
        
        public void RaiseCanExecuteChanged()
        {
            CanExecuteChanged?.Invoke(this, EventArgs.Empty);
        }

        public bool CanExecute(object parameter) => bCanExecute == null || bCanExecute();
        public void Execute(object parameter) => execute();
    }
}

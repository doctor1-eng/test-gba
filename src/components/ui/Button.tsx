import type { ButtonHTMLAttributes } from 'react';
import './ui.css';

interface ButtonProps extends ButtonHTMLAttributes<HTMLButtonElement> {
  variant?: 'primary' | 'secondary' | 'danger';
  block?: boolean;
  small?: boolean;
}

export function Button({ variant = 'secondary', block, small, className = '', children, ...rest }: ButtonProps) {
  const classes = [
    'btn',
    variant === 'primary' ? 'btn-primary' : '',
    variant === 'danger' ? 'btn-danger' : '',
    block ? 'btn-block' : '',
    small ? 'btn-sm' : '',
    className,
  ].filter(Boolean).join(' ');
  return (
    <button className={classes} {...rest}>
      {children}
    </button>
  );
}

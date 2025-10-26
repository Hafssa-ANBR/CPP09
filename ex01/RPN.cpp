/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanebaro <hanebaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 15:33:54 by hanebaro          #+#    #+#             */
/*   Updated: 2025/10/26 17:37:56 by hanebaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const RPN &cpy)
{
    operands = cpy.operands;
}

RPN& RPN::operator=(const RPN &affect)
{
    if (this != &affect)
        operands = affect.operands;
    return (*this);
}

RPN::~RPN() {}

bool RPN::isOperator(char c)
{
    if(c == '+' || c == '-' || c == '*' || c == '/')
        return (true);
    return (false);
}

bool RPN::isNumber(char c)
{
    if (std::isdigit(c))
        return (true);
    return (false);
}

int RPN::applyOperator(char op, int a, int b)
{
    switch(op)
    {
        case '+':
            return (a + b);
        case '-':
            return (a - b);
        case '*':
            return (a * b);
        case '/':
            if (b == 0)
                throw std::runtime_error("Division by zero");
            return (a / b);
        default:
            throw std::runtime_error("Invalid operator");
    }
}

int RPN::evaluate(const std::string& expression)
{
    for(size_t i = 0; i < expression.length(); ++i)
    {
        char token = expression[i];
        
        if (token == ' ' || token == '\t')
            continue;
        
        if (isNumber(token))
        {
            operands.push(token - '0');
        }
        else if (isOperator(token))
        {
            if (operands.size() < 2)
                throw std::runtime_error("Error");
            
            int b = operands.top();
            operands.pop();
            int a = operands.top();
            operands.pop();
            int result = applyOperator(token, a, b);
            operands.push(result);
        }
        else
        {
            throw std::runtime_error("Error");
        }
    }
    if (operands.size() != 1)
        throw std::runtime_error("Error");
    return(operands.top());
}
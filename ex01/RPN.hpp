/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanebaro <hanebaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 15:33:57 by hanebaro          #+#    #+#             */
/*   Updated: 2025/10/26 17:05:54 by hanebaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <string>
#include <stack>

class RPN 
{
    private:
        std::stack<int> operands;
        bool isOperator(char c);
        bool isNumber(char c);
        int applyOperator(char op, int a, int b);
    public:
        RPN();
        RPN(const RPN &cpy);
        RPN& operator=(const RPN &affect);
        ~RPN();
        int evaluate(const std::string& expression);

};
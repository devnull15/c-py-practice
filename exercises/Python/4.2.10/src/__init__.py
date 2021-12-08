import argparse
def parse_cli(args):
    """
    Using the argparse module, create a parser the handle the command line 
    arguments, args.

    You should take 2 positional arguments, x which is the base and y which 
    is the exponent.

    You should also take an optional argument "-m" or "--modulus" which is
    the modulus

    All arguments will be integer values.

    args: args A list of command line arguments
    return: (x ^ y) mod m
    """
    parser = argparse.ArgumentParser(description='(x^y) mod m')
    parser.add_argument('x', type=int, help='base')
    parser.add_argument('y', type=int, help='exponent')
    parser.add_argument('--modulus', '-m', type=int, help='modulus', required=False, nargs=1)
    eq = vars(parser.parse_args(args))
    if(eq['modulus'] != None):
        ret = (eq['x']**eq['y']) % eq['modulus'][0]
    else:
        ret = (eq['x']**eq['y'])
    return ret

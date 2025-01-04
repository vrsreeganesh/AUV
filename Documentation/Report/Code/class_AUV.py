# class representing AUV
class AUV:
    # init function
    def __init__(self,
                    location,              # current location of AUV [tensor]
                    velocity,              # velocity of AUV [tensor]
                    acceleration,          # acceleration of AUV [tensor]
                    pointing_direction):   # direction in which AUV is pointed [tensor]
        """
        Initializing parameters related to AUV
        """
        # fundamental attributes
        self.location               = location              # current location of AUV
        self.velocity               = velocity              # velocity of AUV
        self.acceleration           = acceleration          # acceleration of AUV
        self.pointing_direction     = pointing_direction    # direction in which AUV is pointed

        # add-on attributes
        self.projector_starboard    = None   # projector to the right
        self.projector_portside     = None   # projector to the left
        self.projector_fbls         = None   # projector to the front
        
        self.ula_portside           = None   # ULA mounted on the left
        self.ula_starboard          = None   # ULA mounted on the right

    def summarize(self):
        print(">location              = \n", self.location)
        print(">velocity              = \n", self.velocity)
        print(">acceleration          = \n", self.acceleration)
        print(">pointing_direction    = \n", self.pointing_direction)
        # print("\t>")
        # print("\t>")
        # print("\t>")
        # print("\t>")
        

    def update_timestep(self):
        """
        Updating the after each time step
        """

    def simulate_signal(self):
        """
        Aim: Simulate signals
        Note:
            > Project signals from projectors
            > Return signals from scatters
            > simulate signals received by ULAs
        """
        pass